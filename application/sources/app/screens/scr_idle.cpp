#include "scr_gameplay.h"
#include "game.h"
#include "goldenspiral.h"
#include <functional>
#include <vector>
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

std::vector<GoldenSpiral> spirals;

bool isIdleInit = false;

void initSpiral()
{
    float x = 15 + ((float)rand() / RAND_MAX) * (110 - 15);
    float y = 15 + ((float)rand() / RAND_MAX) * (50 - 15);
    spirals.push_back(GoldenSpiral(x, y));
    timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_UPDATE, 100, TIMER_PERIODIC);
}
void enterIdleScreen()
{
    if (isIdleInit)
        return;
    isIdleInit = true;
    spirals.clear();
    initSpiral();
}
void backToMainScreen()
{
    task_post_pure_msg(AC_TASK_IDLE_ID, AC_IDLE_RESET);
    isIdleInit = false;

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
bool canSpawnAt(float nx, float ny)
{
    for (auto &s : spirals)
    {
        float dx = nx - s.x;
        float dy = ny - s.y;
        float d2 = dx * dx + dy * dy;
        // safe zone radius
        if (d2 < (20 * 20))
            return false; // 20 pixel
    }
    return true;
}
void renderGoldenSpirals(const GoldenSpiral &s)
{
    float a = 0;
    while (a < s.angle)
    {
        float r = s.k * a;
        int px = s.x + cos(a) * r;
        int py = s.y + sin(a) * r;
        view_render.drawPixel(px, py, WHITE);
        a += 0.05;
    }
}
// --- Render ---
void view_scr_idle()
{
    if (spirals.size() < 4)
    {
        if (rand() % 30 == 0)
        {
            for (int tries = 0; tries < 10; tries++)
            {
                float nx = 15 + ((float)rand() / RAND_MAX) * (110 - 15);
                float ny = 15 + ((float)rand() / RAND_MAX) * (50 - 15);

                if (canSpawnAt(nx, ny))
                {
                    spirals.push_back(GoldenSpiral(nx, ny));
                    break;
                }
            }
        }
    }
    // // update + render
    for (int i = spirals.size() - 1; i >= 0; i--)
    {
        if (!spirals[i].update())
            spirals.erase(spirals.begin() + i);
        else
            renderGoldenSpirals(spirals[i]);
    }
}
// --- Handle ---
void scr_idle_handle(ak_msg_t *msg)
{
    switch (msg->sig)
    {
    case SCREEN_ENTRY:

        enterIdleScreen();
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
    case AC_DISPLAY_UPDATE:
        break;
    default:
        break;
    }
}
