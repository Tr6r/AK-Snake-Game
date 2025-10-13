#ifndef __SCR_TIMER_SETTING_H__
#define __SCR_TIMER_SETTING_H__

#include "fsm.h"
#include "port.h"
#include "message.h"
#include "timer.h"

#include "sys_ctrl.h"
#include "sys_dbg.h"

#include "app.h"
#include "app_dbg.h"
#include "task_list.h"
#include "task_log.h"

#include "task_display.h"
#include "view_render.h"
#include <string>
// #include "app_rf24.h"

#include "buzzer.h"

extern view_dynamic_t dyn_view_timer_setting;

extern view_screen_t scr_timer_setting;
extern void scr_timer_setting_handle(ak_msg_t* msg);

#endif //__SCR_TIMER_SETTING_H__
