#ifndef __SCR_TIMER_H__
#define __SCR_TIMER_H__

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
#include "task_log.h"
#include "view_render.h"
#include <string>

#include "buzzer.h"
#include "eeprom.h"
#include "app_eeprom.h"

enum dw_state
{
    DW_NORMAL, 
    DW_SET_HOUR,
    DW_SET_MINUTE,
    DW_SET_SECOND,
    
};
typedef struct {
    int cur;        // giá trị hiện tại
    int old;        // giá trị cũ (để vẽ song song)
    bool anim;      // đang animate?
    int anim_y;     // offset Y (âm thì nằm trên)
} digit_anim_t;
extern view_dynamic_t dyn_view_timer;

extern view_screen_t scr_timer;
extern void scr_timer_handle(ak_msg_t *msg);

#endif //__SCR_TIMER_H__
