#include "fsm.h"
#include "port.h"
#include "message.h"
#include "timer.h"

#include "app.h"
#include "app_dbg.h"
#include "task_list.h"
#include "task_display.h"
void task_idle(ak_msg_t *msg)
{
	switch (msg->sig)
	{
	case AC_IDLE_INIT:
		timer_set(AC_TASK_IDLE_ID, AC_DISPLAY_SHOW_IDLE, 5000, TIMER_PERIODIC);

		break;
	case AC_IDLE_RESET:
		timer_set(AC_TASK_IDLE_ID, AC_DISPLAY_SHOW_IDLE, 5000, TIMER_PERIODIC);

		break;
	case AC_IDLE_DEL:
		timer_remove_attr(AC_TASK_IDLE_ID, AC_DISPLAY_SHOW_IDLE);

		break;
	case AC_DISPLAY_SHOW_IDLE:

        SCREEN_TRAN(scr_idle_handle, &scr_idle);

        break;
	default:
		break;
	}
}
