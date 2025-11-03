#include "scr_menu.h"
#include "game.h"
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
bool isEeproomInit = false;
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

enum MenuItem : uint8_t
{
    MENU_PLAY = 0,
    MENU_ABOUT,
    MENU_COUNT // tổng số menu
};

static const char *menu_text[MENU_COUNT] = {
    "Play",
    "Config"};
void drawFrameBlock(int x, int y)
{
    for (int j = 0; j < 2; j++)
    {
        for (int i = 0; i < 2; i++)
        {
            if (frame_block[j] & (1 << (1 - i)))
            {
                view_render.drawPixel(x + i, y + j, WHITE);
            }
        }
    }
}
void renderFrameBlock(int x, int y, int w, int h)
{
    const int blockSize = 2;
    int maxX = x + w;
    int maxY = y + h;

    // Cạnh trên & dưới
    for (int i = x; i < maxX; i += blockSize)
    {
        drawFrameBlock(i, y);                // top
        drawFrameBlock(i, maxY - blockSize); // bottom
    }

    // Cạnh trái & phải
    for (int j = y; j < maxY; j += blockSize)
    {
        drawFrameBlock(x, j);                // left
        drawFrameBlock(maxX - blockSize, j); // right
    }
    // view_render.fillRect(70,20, 30, 20, WHITE);
}
void renderArrow()
{
    int arrowX = 10;
    int arrowY = 30;
    if (isBlink)
        view_render.fillTriangle(arrowX, arrowY - 5, arrowX, arrowY + 5, arrowX + 6, arrowY, WHITE);
    else
        view_render.fillTriangle(arrowX, arrowY - 5, arrowX, arrowY + 5, arrowX + 6, arrowY, BLACK);
}
void renderScrollbar()
{
    // Thanh cuộn nằm trong vùng cao 64px
    const int scrollbarX = 118;
    const int scrollbarY = 0;
    const int scrollbarW = 10;
    const int scrollbarH = 64;

    // Chiều cao con trượt (thumb)
    int thumbH = scrollbarH / MENU_COUNT; // ví dụ: 3 mục => ~21px/mục
    if (thumbH < 6)
        thumbH = 6; // đảm bảo không quá nhỏ

    // Vị trí cơ sở (theo menu_index)
    float ratio = (float)menu_index + ((float)anim_offset / 32.0f); // 32 = charHeight*2
    float posRatio = ratio / (MENU_COUNT - 1);
    if (posRatio < 0)
        posRatio = 0;
    if (posRatio > 1)
        posRatio = 1;

    int thumbY = scrollbarY + (int)((scrollbarH - thumbH) * posRatio);

    // Vẽ thanh nền
    view_render.drawRect(scrollbarX, scrollbarY, scrollbarW, scrollbarH, WHITE);
    // Vẽ phần thumb
    view_render.fillRect(scrollbarX + 1, thumbY + 1, scrollbarW - 2, thumbH - 2, WHITE);
}

void renderItem(int charHeight, int centerY)
{
    for (int i = -1; i <= 1; i++)
    {
        int item_index = (int)menu_index + i;
        if (item_index < 0 || item_index >= MENU_COUNT)
            continue;

        int y = centerY + 4 + (i * charHeight * 2) - anim_offset;
        const char *text = menu_text[item_index];
        int x = 30;
        view_render.setCursor(x, y);
        view_render.print(text);
        int icon_x = 70;
        int icon_y;
        // Render icon theo từng item thực tế
        switch (item_index)
        {
        case 0: // Gameplay
            icon_y = y - 5;

            view_render.drawBitmap(icon_x, icon_y, gameplay_bitmap, 30, 20, WHITE);
            break;

        case 1: // Config
            icon_y = y - 6;

            view_render.drawBitmap(icon_x, icon_y, config_bitmap, 30, 20, WHITE);
            break;

        case 2: // High Score (ví dụ)
            icon_y = y - 6;

            view_render.drawBitmap(icon_x, icon_y, top_bitmap, 30, 20, WHITE);
            break;

        default:
            // Nếu chưa có icon thì bỏ qua
            break;
        }
    }
}
void anim(int charHeight, int centerY)
{
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
}
// --- Hàm render ---
void view_scr_menu()
{
    view_render.fillRect(0, 0, 128, 64, BLACK);
    view_render.drawRect(0, 0, 128, 64, WHITE);
    renderFrameBlock(1, 15, 116, 30);
    view_render.setTextSize(1);
    view_render.setTextColor(WHITE);

    renderArrow();
    int charHeight = 16;
    int centerY = 24;

    // Cập nhật animation
    anim(charHeight, centerY);

    // Vẽ 3 item (trên, giữa, dưới)
    renderItem(charHeight, centerY);
    renderScrollbar();
}
void scr_menu_handle(ak_msg_t *msg)
{
    switch (msg->sig)
    {
    case SCREEN_ENTRY:
    if(!isEeproomInit)
    {
        game.loadConfig();
        isEeproomInit = true;
    }
        menu_index = 0;
        anim_offset = 0;
        anim_active = false;
        timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_UPDATE, 100, TIMER_PERIODIC);
        break;

    case AC_DISPLAY_BUTON_UP_PRESS:
        if (!anim_active && menu_index > 0)
        {
            if (game.getIsAudio())

                BUZZER_PlayTones(tones_menu_click);

            anim_direction = -1;
            anim_offset = 0;
            anim_active = true;
        }
        break;

    case AC_DISPLAY_BUTON_DOWN_PRESS:
        if (!anim_active && menu_index < MENU_COUNT - 1)
        {
            if (game.getIsAudio())

                BUZZER_PlayTones(tones_menu_click);

            anim_direction = +1;
            anim_offset = 0;
            anim_active = true;
        }
        break;

    case AC_DISPLAY_BUTON_MODE_PRESS:
        // APP_DBG_SIG("Chọn mục: %s\n", menu_text[menu_index]);
        if (game.getIsAudio())

            BUZZER_PlayTones(tones_menu_select);

        if (menu_index == 0)
        {
            // timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_UPDATE);
            // task_post_pure_msg(AC_TASK_GAME_ID, AC_GAME_INIT);

            SCREEN_TRAN(scr_gamemap_handle, &scr_gamemap);
            // SCREEN_TRAN(scr_timer_handle, &scr_timer);
        }
        else
        {
            // timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_UPDATE);

            SCREEN_TRAN(scr_config_handle, &scr_config);
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