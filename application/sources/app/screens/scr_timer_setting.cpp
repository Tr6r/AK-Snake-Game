#include "scr_timer_setting.h"
using namespace std;

static void view_scr_timer_setting();

view_dynamic_t dyn_view_timer_setting = {
	{
		.item_type = ITEM_TYPE_DYNAMIC,
	},
	view_scr_timer_setting};

view_screen_t scr_timer_setting = {
	&dyn_view_timer_setting,
	ITEM_NULL,
	ITEM_NULL,

	.focus_item = 0,
};
uint8_t skip_button = false;
int flag = 0;
void view_scr_timer_setting()
{
	char buf[10]; // buffer chuỗi

	// clear vùng hiển thị cũ
	view_render.fillRect(0, 0, 128, 64, BLACK);

	// Vẽ khung (nếu cần)
	view_render.drawRect(0, 0, 128, 64, WHITE);

	sprintf(buf, "%d", flag); // In số nguyên flag

	view_render.setTextSize(2);
	view_render.setTextColor(WHITE);

	// Tính toán để căn giữa
	int charWidth = 6 * 2; // font 6px, scale x2
	int textWidth = strlen(buf) * charWidth;
	int x = (128 - textWidth) / 2;

	int charHeight = 8 * 2; // font 8px, scale x2
	int y = (64 - charHeight) / 2;

	view_render.setCursor(x, y);
	view_render.print(buf);
}
void scr_timer_setting_handle(ak_msg_t *msg)
{
	switch (msg->sig)
	{
	case SCREEN_ENTRY:
	{
		APP_DBG_SIG("SCREEN_ENTRY\n");
		timer_set(AC_TASK_DISPLAY_ID,
				  AC_DISPLAY_TEST,
				  100, TIMER_ONE_SHOT);
	}
	break;

	case AC_DISPLAY_BUTON_MODE_RELEASED:
	{
		if (skip_button)
		{
			skip_button = false;
			return;
		}
		APP_DBG_SIG("AC_DISPLAY_BUTON_MODE_RELEASED4\n");
		SCREEN_TRAN(scr_timer_handle, &scr_timer);
	}
	break;

	case AC_DISPLAY_BUTON_UP_RELEASED:
	{
		if (skip_button)
		{
			skip_button = false;
			return;
		}
		APP_DBG_SIG("AC_PRINT_LOG\n");

		task_post_pure_msg(AC_TASK_LOG_ID, AC_LOG_PRINT);
	}
	break;

	case AC_DISPLAY_BUTON_DOWN_RELEASED:
	{
		if (skip_button)
		{
			skip_button = false;
			return;
		}
		APP_DBG_SIG("AC_PUT_LOG\n");
		task_post_pure_msg(AC_TASK_LOG_ID, AC_LOG_PUT);
	}
	break;
	case AC_DISPLAY_BUTON_DOWN_LONG_RELEASED:
	{
		APP_DBG_SIG("AC_PUT_LOG_TO_EEPROM\n");
		skip_button = true;
		task_post_pure_msg(AC_TASK_LOG_ID, AC_LOG_PUT_EEPROM);
	}
	break;
	case AC_DISPLAY_BUTON_UP_LONG_RELEASED:
	{
		APP_DBG_SIG("AC_PRINT_LOG_TO_EEPROM\n");

		skip_button = true;

		task_post_pure_msg(AC_TASK_LOG_ID, AC_LOG_PRINT_EEPROM);
	}
	break;

	case AC_DISPLAY_TEST:
	{
		APP_DBG_SIG("AC_PUT_LOG_TO_EEPROM111\n");

		// task_post_pure_msg(AC_TASK_LOG_ID, AC_LOG_PRINT_EEPROM);
	}
	break;
	case AC_DISPLAY_BUTON_MODE_LONG_RELEASED:
	{
		APP_DBG_SIG("AC_CLEAR_LOG_EEPROM\n");
		skip_button = true;
		task_post_pure_msg(AC_TASK_LOG_ID, AC_CLEAR_LOG_EEPROM);
	}
	break;
	default:
		break;
	}
}
