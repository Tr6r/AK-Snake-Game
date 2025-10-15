#ifndef __SCR_MENU_H__
#define __SCR_MENU_H__

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

#include "buzzer.h"

class MenuScreen
{
private:
    std::string pre_text;
    std::string cur_text;
    int anim_y;
    bool anim;
};

extern view_dynamic_t dyn_view_menu;

extern view_screen_t scr_menu;
extern void scr_menu_handle(ak_msg_t *msg);

#endif //__SCR_MENU_H__
