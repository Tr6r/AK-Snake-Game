
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

typedef struct gameplay_t
{
    uint8_t apple_blink_count = 0;
    uint8_t frame_count = 0;
    uint16_t fps_timer = 0;
    uint8_t fps = 0;
    bool apple_blink_state = true;
} gameplay_t;
extern view_dynamic_t dyn_view_gameplay;

extern view_screen_t scr_gameplay;
extern void scr_gameplay_handle(ak_msg_t *msg);

#endif //__SCR_GAMEPLAY_H__
