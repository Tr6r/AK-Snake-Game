
#include "scr_timer.h"
#include "rtc.h"

static void view_scr_timer();
dw_state state;
digit_anim_t clock_digits[6];

bool skipNextRelease = false;
bool blinkTime = false;
Time realTime;
Date realDate;

int charWidth = 12;
int charHeight = 16;
int y_pos = 24;

void nextStatus(void)
{
	switch (state)
	{
	case DW_NORMAL:
	{
		state = DW_SET_HOUR;
		// timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_TIMER_UPDATE);
		timer_set(AC_TASK_DISPLAY_ID,
				  AC_DISPLAY_BLINK_UPDATE,
				  200, TIMER_PERIODIC);
		break;
	}
	case DW_SET_HOUR:
	{
		state = DW_SET_MINUTE;
		break;
	}
	case DW_SET_MINUTE:
	{
		state = DW_SET_SECOND;
		break;
	}
	case DW_SET_SECOND:
	{

		realTime.hh = clock_digits[0].cur * 10 + clock_digits[1].cur;
		realTime.mm = clock_digits[2].cur * 10 + clock_digits[3].cur;
		realTime.ss = clock_digits[4].cur * 10 + clock_digits[5].cur;

		// Ghi thời gian mới vào RTC
		rtc_set_time(&realTime);

		timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_BLINK_UPDATE);
		blinkTime = false;
		skipNextRelease = true;
		state = DW_NORMAL;

		break;
	}
	default:
		break;
	}
}
void increment(void)
{
	if (state == DW_NORMAL)
		return;

	switch (state)
	{
	case DW_SET_HOUR:
	{
		int hh = clock_digits[0].cur * 10 + clock_digits[1].cur;
		hh = (hh + 1) % 24;
		clock_digits[0].cur = hh / 10;
		clock_digits[1].cur = hh % 10;
		break;
	}
	case DW_SET_MINUTE:
	{
		int mm = clock_digits[2].cur * 10 + clock_digits[3].cur;
		mm = (mm + 1) % 60;
		clock_digits[2].cur = mm / 10;
		clock_digits[3].cur = mm % 10;
		break;
	}
	case DW_SET_SECOND:
	{
		int ss = clock_digits[4].cur * 10 + clock_digits[5].cur;
		ss = (ss + 1) % 60;
		clock_digits[4].cur = ss / 10;
		clock_digits[5].cur = ss % 10;
		break;
	}
	default:
		break;
	}
}

void decrement(void)
{
	if (state == DW_NORMAL)
		return;

	switch (state)
	{
	case DW_SET_HOUR:
	{
		int hh = clock_digits[0].cur * 10 + clock_digits[1].cur;
		hh = (hh - 1 + 24) % 24; // tránh âm
		clock_digits[0].cur = hh / 10;
		clock_digits[1].cur = hh % 10;
		break;
	}
	case DW_SET_MINUTE:
	{
		int mm = clock_digits[2].cur * 10 + clock_digits[3].cur;
		mm = (mm - 1 + 60) % 60;
		clock_digits[2].cur = mm / 10;
		clock_digits[3].cur = mm % 10;
		break;
	}
	case DW_SET_SECOND:
	{
		int ss = clock_digits[4].cur * 10 + clock_digits[5].cur;
		ss = (ss - 1 + 60) % 60;
		clock_digits[4].cur = ss / 10;
		clock_digits[5].cur = ss % 10;
		break;
	}
	default:
		break;
	}
}

view_dynamic_t dyn_view_timer = {
	{
		.item_type = ITEM_TYPE_DYNAMIC,
	},
	view_scr_timer};

view_screen_t scr_timer = {
	&dyn_view_timer,
	ITEM_NULL,
	ITEM_NULL,
	.focus_item = 0,
};
class line_segment
{
public:
	int x0, x1;
	int distance()
	{
		return (x1 - x0);
	}
};

// Hàm animate 1 digit
void render_anim_digit_ex(digit_anim_t *d, int x, int y_pos, int charWidth, int charHeight)
{
	char buf[2];
	// Nếu khác thì bắt đầu animate
	if (d->old != d->cur && !d->anim)
	{
		// temp = d->old;
		// d->old = d->cur;		 // lưu lại số cũ
		d->anim_y = -charHeight; // số mới từ trên
		d->anim = true;
	}

	// Xóa vùng vẽ
	view_render.fillRect(x, y_pos - charHeight, charWidth, charHeight * 2, BLACK);

	if (d->anim)
	{
		// vẽ số mới
		sprintf(buf, "%d", d->cur);
		view_render.setCursor(x, y_pos + d->anim_y);
		view_render.print(buf);

		// vẽ số cũ (song song với số mới, lệch 1 chiều cao)
		sprintf(buf, "%d", d->old);
		view_render.setCursor(x, y_pos + d->anim_y + charHeight);
		view_render.print(buf);
		d->anim_y += 4; // tốc độ

		// kết thúc khi số mới vừa khớp vị trí chuẩn
		if (d->anim_y >= 0)
		{
			d->old = d->cur; // lưu lại số cũ

			d->anim = false;
			d->anim_y = 0;
		}
	}
	else
	{
		// vẽ số hiện tại
		sprintf(buf, "%d", d->cur);
		view_render.setCursor(x, y_pos);
		view_render.print(buf);
	}
}

// Animate 2 chữ số (ví dụ HH, MM, SS)
void render_anim_number2digits_ex(digit_anim_t *tens, digit_anim_t *units, int x, int y_pos, int charWidth, int charHeight, bool blink)
{

	if (blink)
	{
		// render đơn vị trước, rồi đến hàng chục
		render_anim_digit_ex(units, x + charWidth, y_pos, charWidth, charHeight);
		render_anim_digit_ex(tens, x, y_pos, charWidth, charHeight);
	}
	else
	{
		// blink: không vẽ số, chỉ vẽ khoảng trắng
		view_render.fillRect(x, y_pos, charWidth, charHeight, BLACK);
		view_render.fillRect(x + charWidth, y_pos, charWidth, charHeight, BLACK);
	}
}
// Hàm chính hiển thị clock
void view_scr_timer()
{
	view_render.drawRect(0, 0, 128, 64, WHITE);

	view_render.setTextSize(2);
	view_render.setTextColor(WHITE);

	// HH
	bool blink_hh = (state == DW_SET_HOUR) ? blinkTime : true;
	render_anim_number2digits_ex(
		&clock_digits[0], &clock_digits[1],
		20, y_pos, charWidth, charHeight,
		blink_hh);

	// :
	view_render.setCursor(44, y_pos);
	view_render.print(":");

	// MM
	bool blink_mm = (state == DW_SET_MINUTE) ? blinkTime : true;
	render_anim_number2digits_ex(
		&clock_digits[2], &clock_digits[3],
		52, y_pos, charWidth, charHeight,
		blink_mm);

	// :
	view_render.setCursor(76, y_pos);
	view_render.print(":");

	// SS
	bool blink_ss = (state == DW_SET_SECOND) ? blinkTime : true;
	render_anim_number2digits_ex(
		&clock_digits[4], &clock_digits[5],
		84, y_pos, charWidth, charHeight,
		blink_ss);
}
void rt_to_dig(Time *t)
{
	// HH
	clock_digits[0].cur = t->hh / 10;
	clock_digits[1].cur = t->hh % 10;

	// MM
	clock_digits[2].cur = t->mm / 10;
	clock_digits[3].cur = t->mm % 10;

	// SS
	clock_digits[4].cur = t->ss / 10;
	clock_digits[5].cur = t->ss % 10;
}
void check_and_update_animation(void)
{
	if (state == DW_NORMAL)
	{
		realTime = rtc_get_time(); // lấy thời gian hiện tại từ RTC
// 		char buffi[16];			   // đủ lớn để chứa "hh : mm : ss" + '\0'

// 		sprintf(buffi, "%02d : %02d : %02d", realTime.hh, realTime.mm, realTime.ss);
// APP_DBG_SIG("%s\n", buffi);

		rt_to_dig(&realTime);
	}
}
void init_dw(void)
{

	for (int i = 0; i < 6; i++)
	{
		clock_digits[i].cur = 0;
		clock_digits[i].old = 0;
		clock_digits[i].anim = false;
		clock_digits[i].anim_y = 0;
	}
}
void scr_timer_handle(ak_msg_t *msg)
{
	switch (msg->sig)
	{
	case SCREEN_ENTRY:
	{
		APP_DBG_SIG("SCREEN_ENTRY (scr_timer)\n");
		timer_set(AC_TASK_DISPLAY_ID,
				  AC_DISPLAY_TIMER_UPDATE,
				  100,
				  TIMER_PERIODIC);
		// reset trạng thái anim
		init_dw();

	}
	break;
	case AC_DISPLAY_TIMER_UPDATE:
	{

		check_and_update_animation();
	}
	break;
	case AC_DISPLAY_BUTON_UP_RELEASED:
	{
		APP_DBG_SIG("AC_DISPLAY_TIMER_INCREASE\n");
		increment();
	}
	break;
	case AC_DISPLAY_BUTON_DOWN_RELEASED:
	{
		APP_DBG_SIG("AC_DISPLAY_TIMER_DECREASE\n");
		decrement();
	}
	break;
	case AC_DISPLAY_BUTON_MODE_LONG_RELEASED:
	{
		skipNextRelease = true;
		if (state == DW_NORMAL)
		{
			APP_DBG_SIG("AC_DISPLAY_TIMER_UPDATING\n");
			nextStatus();
		}
		else
		{
			state = DW_NORMAL;
			timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_BLINK_UPDATE);
		}
	}
	break;
	case AC_DISPLAY_BUTON_UP_LONG_RELEASED:
	{
		APP_DBG_SIG("AC_PRINT_LOG\n");

		task_post_pure_msg(AC_TASK_LOG_ID, AC_LOG_PRINT);
	}
	break;
	case AC_DISPLAY_BUTON_DOWN_LONG_RELEASED:
	{
		APP_DBG_SIG("AC_PUT_LOG\n");

		task_post_pure_msg(AC_TASK_LOG_ID, AC_LOG_PUT);

	}
	break;
	case AC_DISPLAY_BUTON_MODE_RELEASED:
	{
		if (!skipNextRelease && state != DW_NORMAL)
		{
			nextStatus();
		}
		if (!skipNextRelease && state == DW_NORMAL)
		{
			timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_TIMER_UPDATE);
			SCREEN_TRAN(scr_timer_setting_handle, &scr_timer_setting);
		}
		skipNextRelease = false;
	}
	break;
	case AC_DISPLAY_BLINK_UPDATE:
	{
		blinkTime = !blinkTime;
	}
	break;
	default:
		break;
	}
}