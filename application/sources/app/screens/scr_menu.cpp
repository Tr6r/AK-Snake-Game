#include "scr_menu.h"
#include "scr_gameplay.h"
#include <vector>
#include <string>
using namespace std;

static void view_scr_menu();

// --- Cấu hình menu ---

int menu_index = 0;
int anim_offset = 0;
bool anim_active = false;
int anim_direction = 0; // +1: xuống, -1: lên
uint8_t anim_speed = 10;

bool isBlink = false;
uint8_t blinkCount = 0;
int arrowX = 5;
int arrowY = 30; // giữa vùng (15–45)

// --- Đăng ký dynamic view ---
view_dynamic_t dyn_view_menu = {
    {.item_type = ITEM_TYPE_DYNAMIC},
    view_scr_menu};

view_screen_t scr_menu = {
    &dyn_view_menu,
    ITEM_NULL,
    ITEM_NULL,
    .focus_item = 0,
};

enum MenuItem : uint8_t {
    MENU_PLAY = 0,
    MENU_CONFIG,
    MENU_ABOUT,
    MENU_COUNT  // tổng số menu
};

static const char* menu_text[MENU_COUNT] = {
    "Play",
    "Config",
    "About"
};

// --- Hàm render --- 
void view_scr_menu()
{
    view_render.fillRect(0, 0, 128, 64, BLACK);
    view_render.drawRect(0, 15, 128, 30, WHITE);
    view_render.setTextSize(2);
    view_render.setTextColor(WHITE);

    int arrowX = 10;
    int arrowY = 30;
    if (isBlink)
        view_render.fillTriangle(arrowX, arrowY - 5, arrowX, arrowY + 5, arrowX + 6, arrowY, WHITE);
    else
        view_render.fillTriangle(arrowX, arrowY - 5, arrowX, arrowY + 5, arrowX + 6, arrowY, BLACK);

    int charHeight = 16;
    int centerY = 24;

    // Cập nhật animation
    if (anim_active)
    {
        anim_offset += anim_direction * anim_speed;
        if (abs(anim_offset) >= charHeight * 2)
        {
            anim_offset = 0;
            anim_active = false;
            menu_index += anim_direction;
        }
    }

    // Vẽ 3 item (trên, giữa, dưới)
    for (int i = -1; i <= 1; i++)
    {
        int item_index = (int)menu_index + i;
        if (item_index < 0 || item_index >= MENU_COUNT)
            continue;

        int y = centerY + (i * charHeight * 2) - anim_offset;
        const char* text = menu_text[item_index];
        int x = (128 - strlen(text) * 12) / 2;

        view_render.setCursor(x, y);
        view_render.print(text);
    }
}
void scr_menu_handle(ak_msg_t *msg)
{
    switch (msg->sig)
    {
    case SCREEN_ENTRY:
        menu_index = 0;
        anim_offset = 0;
        anim_active = false;
        timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_UPDATE, 100, TIMER_PERIODIC);
        break;

    case AC_DISPLAY_BUTON_UP_RELEASED:
        if (!anim_active && menu_index > 0)
        {
            anim_direction = -1;
            anim_offset = 0;
            anim_active = true;
        }
        break;

    case AC_DISPLAY_BUTON_DOWN_RELEASED:
        if (!anim_active && menu_index < MENU_COUNT - 1)
        {
            anim_direction = +1;
            anim_offset = 0;
            anim_active = true;
        }
        break;

    case AC_DISPLAY_BUTON_MODE_RELEASED:
        APP_DBG_SIG("Chọn mục: %s\n", menu_text[menu_index]);
        if (menu_index == 0)
        {
            timer_remove_attr(AC_TASK_DISPLAY_ID,AC_DISPLAY_UPDATE);
            SCREEN_TRAN(scr_gameplay_handle, &scr_gameplay);
        }
        break;

    case AC_DISPLAY_UPDATE:
        blinkCount++;
        if (blinkCount >= 8)
        {
            isBlink = !isBlink;
            blinkCount = 0;
        }
        break;

    default:
        break;
    }
}