
#ifndef __SCR_GAMEPLAY_H__
#define __SCR_GAMEPLAY_H__

#include "fsm.h"
#include "port.h"
#include "message.h"
#include "timer.h"

#include "sys_ctrl.h"
#include "sys_dbg.h"

#include "app.h"
#include "app_dbg.h"
#include "task_list.h"


#include "task_display.h"
#include "view_render.h"
#include <string>

#include "buzzer.h"
#include <stdint.h>
#include <stdbool.h>
#include <functional>


extern view_dynamic_t dyn_view_gameplay;

extern view_screen_t scr_gameplay;
extern void scr_gameplay_handle(ak_msg_t *msg);

#endif //__SCR_GAMEPLAY_H__
