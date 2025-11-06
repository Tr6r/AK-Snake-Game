
#include "scr_gameplay.h"
#include "game.h"
#include "goldenspiral.h"
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
GoldenSpiral sp(30.0, 30.0);

void render(float x,float y)
{
    // từ 0 → angle hiện tại
    float a = 0;
    while (a < sp.angle)
    {
        float r = sp.k * a;
        int px = x + cos(a) * r;
        int py = y + sin(a) * r;
        view_render.drawPixel(px, py, WHITE);
        a += 0.05;
    }
}
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

    if (!sp.update()) {
        sp.reset(); // loop again
xprintf("ssaaa");
    }
    render(sp.x,sp.y);
}

// --- Handle ---
void scr_idle_handle(ak_msg_t *msg)
{
    switch (msg->sig)
    {
    case SCREEN_ENTRY:
        timer_set(AC_TASK_DISPLAY_ID,AC_DISPLAY_IDLE_UPDATE,150,TIMER_PERIODIC);
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
    case AC_DISPLAY_IDLE_UPDATE:
        break;
    default:
        break;
    }
}
