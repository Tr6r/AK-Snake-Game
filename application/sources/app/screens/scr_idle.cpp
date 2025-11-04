
#include "scr_gameplay.h"
#include "game.h"

#include <functional>
using namespace std;

// --- Dynamic view ---
static void view_scr_idle();
view_dynamic_t dyn_view_idle = {
    {.item_type = ITEM_TYPE_DYNAMIC},
    view_scr_idle};

view_screen_t scr_idle = {
    &dyn_view_idle,
    ITEM_NULL,
    ITEM_NULL,
    .focus_item = 0,
};
void backToMainScreen()
{
    task_post_pure_msg(AC_TASK_IDLE_ID, AC_IDLE_RESET);

    switch (game.gameGetState())
    {
    case GAME_STATE_MENU:

        SCREEN_TRAN(scr_menu_handle, &scr_menu);

        break;
    case GAME_STATE_SETTING:
        SCREEN_TRAN(scr_config_handle, &scr_config);

        break;
    case GAME_STATE_CHOOSE_MAP:
        SCREEN_TRAN(scr_gamemap_handle, &scr_gamemap);

        break;

    case GAME_STATE_GAMEOVER:
        SCREEN_TRAN(scr_gameplay_handle, &scr_gameplay);
        break;

    default:
        break;
    }
}
// --- Render ---
void view_scr_idle()
{

    view_render.setCursor(10, 10);
    view_render.print("IDLE");
}

// --- Handle ---
void scr_idle_handle(ak_msg_t *msg)
{
    switch (msg->sig)
    {
    case SCREEN_ENTRY:

        break;
    case AC_DISPLAY_BUTON_MODE_PRESS:
        backToMainScreen();
        break;
    case AC_DISPLAY_BUTON_UP_PRESS:
        backToMainScreen();
        break;
    case AC_DISPLAY_BUTON_DOWN_PRESS:
        backToMainScreen();
        break;
    default:
        break;
    }
}
