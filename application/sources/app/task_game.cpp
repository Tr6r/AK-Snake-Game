
#include "game.h"
// #include "snake.h"
#include <stdbool.h>

#include "fsm.h"
#include "port.h"
#include "message.h"
#include "timer.h"

#include "xprintf.h"

#include "sys_dbg.h"
#include "sys_ctrl.h"
#include "sys_irq.h"
#include "sys_io.h"

#include "app.h"
#include "app_dbg.h"
#include "task_list.h"
#include "task_display.h"

void task_game(ak_msg_t *msg)
{
    switch (msg->sig)
    {

    case AC_GAME_INIT:
        // game.gameInit();
        break;

    case AC_GAME_START:
        APP_DBG_SIG("AC_GAME_START11\n");

        game.gameChangeState(GAME_STATE_PLAYING);
        xprintf("snake speedddd: %d\n", game.snakeGetSpeed());

        // timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_UPDATE, game.snakeGetSpeed(), TIMER_PERIODIC);
        // timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_UPDATE);
        timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_UPDATE, game.snakeGetSpeed(), TIMER_PERIODIC);

        break;

    case AC_GAME_PAUSE:
        if (game.gameGetState() == GAME_STATE_PLAYING)
        {
            game.gameChangeState(GAME_STATE_PAUSE);
            timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_UPDATE);
        }
        else
        {
            game.gameChangeState(GAME_STATE_PLAYING);
            xprintf("snake speedddd: %d\n", game.snakeGetSpeed());

            timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_UPDATE, game.snakeGetSpeed(), TIMER_PERIODIC);
        }
        break;

    case AC_GAME_UPDATE:
        game.snakeUpdate();

        break;
    case AC_GAME_BUTON_UP_PRESS:
        if (game.snakeGetState() == SNAKE_STATE_MOVING)
        {
            game.ignore_update = true;

            game.snakeChangeState(SNAKE_STATE_TURNING);
            game.snakeTurnLeft();
            game.snakeChangeState(SNAKE_STATE_MOVING);
        }

        break;

    case AC_GAME_BUTON_DOWN_PRESS:
        if (game.snakeGetState() == SNAKE_STATE_MOVING)
        {
            game.ignore_update = true;

            game.snakeChangeState(SNAKE_STATE_TURNING);
            game.snakeTurnRight();
            game.snakeChangeState(SNAKE_STATE_MOVING);
        }

        break;
    default:
        break;
    }
}
