#include "scr_startup.h"
#include "view_render.h"
#include "view_item.h"
static void view_scr_startup();
static void app_init_screen();

view_rectangle_t rect_hello;
view_screen_t scr_hello;

view_dynamic_t dyn_view_startup = {
	{
		.item_type = ITEM_TYPE_DYNAMIC,
	},
	view_scr_startup};

view_screen_t scr_startup = {
	&dyn_view_startup,
	ITEM_NULL,
	ITEM_NULL,

	.focus_item = 0,
};

void view_scr_startup()
{
#define AK_LOGO_AXIS_X (23)
#define AK_LOGO_TEXT (AK_LOGO_AXIS_X + 4)
	/* ak logo */
	view_render.clear();
	view_render.setTextSize(1);
	view_render.setTextColor(WHITE);
	view_render.setCursor(AK_LOGO_AXIS_X, 3);
	view_render.print("   __    _  _ ");
	view_render.setCursor(AK_LOGO_AXIS_X, 10);
	view_render.print("  /__\\  ( )/ )");
	view_render.setCursor(AK_LOGO_AXIS_X, 20);
	view_render.print(" /(__)\\ (   (");
	view_render.setCursor(AK_LOGO_AXIS_X, 30);
	view_render.print("(__)(__)(_)\\_)");
	view_render.setCursor(AK_LOGO_TEXT, 42);
	view_render.print("Active Kernel");
}

void scr_startup_handle(ak_msg_t *msg)
{
	switch (msg->sig)
	{
	case AC_DISPLAY_INITIAL:
	{
		APP_DBG_SIG("AC_DISPLAY_INITIAL\n");
		view_render.initialize();
		view_render_display_on();
		// timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_MENU, AC_DISPLAY_STARTUP_INTERVAL, TIMER_ONE_SHOT);
		// SCREEN_TRAN(scr_idle_handle, &scr_idle);

	}
	break;
	case AC_DISPLAY_SHOW_MENU:
	{
		SCREEN_TRAN(scr_menu_handle, &scr_menu);

	}
	break;
	case AC_DISPLAY_BUTON_MODE_RELEASED:
	{
		APP_DBG_SIG("AC_DISPLAY_BUTON_MODE_RELEASED0000\n");
	}
	break;

	default:
		break;
	}
}
void app_init_screen()
{
	// Khởi tạo rectangle
	rect_hello.item.item_type = ITEM_TYPE_RECTANGLE; // dùng rect_hello.item.item_type
	rect_hello.x = 0;
	rect_hello.y = 0;
	rect_hello.width = 128;
	rect_hello.height = 16;
	rect_hello.font_size = 1;

	// Gán text (vì là mảng nên phải copy chuỗi)
	strcpy(rect_hello.text, "Hello");

	rect_hello.type = BACK_GND_STYLE_FILL;
	rect_hello.text_color = WHITE; // thêm nếu cần màu chữ
	rect_hello.border_width = 0;
	rect_hello.focus_cursor = 0;
	rect_hello.focus_size = 0;

	// Khởi tạo screen
	for (int i = 0; i < NUMBER_SCREEN_ITEMS_MAX; i++)
	{
		scr_hello.item[i] = ITEM_NULL;
	}
	scr_hello.focus_item = 0;
	scr_hello.item[0] = (void *)&rect_hello;
}