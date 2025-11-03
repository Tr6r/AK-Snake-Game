
#ifndef __SCR_GAMEMAP_H__
#define __SCR_GAMEMAP_H__

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
#include <stdint.h>
#include <stdbool.h>
#include <functional>
#define MAP_GRID 4

typedef struct gamemap_t
{
    uint8_t currentMap = 0;    
    uint8_t targetMap = 0;
	
	bool isBlink = false;    
	uint8_t blinkCount = 0;
	bool ignoreNav = false;
	
    int animOffsetX = 0;        
    bool animating = false;      
    int animDir = 0;   
	
}gamemap_t;

extern view_dynamic_t dyn_view_gamemap;

extern view_screen_t scr_gamemap;
extern void scr_gamemap_handle(ak_msg_t *msg);
extern gamemap_t gameMap;

#endif //__SCR_GAMEMAP_H__
