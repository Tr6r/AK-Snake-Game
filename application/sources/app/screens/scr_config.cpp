#include "scr_config.h"
#include "game.h"
using namespace std;

static void view_scr_config();

view_dynamic_t dyn_view_config = {
    {
        .item_type = ITEM_TYPE_DYNAMIC,
    },
    view_scr_config};

view_screen_t scr_config = {
    &dyn_view_config,
    ITEM_NULL,
    ITEM_NULL,

    .focus_item = 0,
};

memu_config_t menuConfig;
const int BITMAP_W = 70;
const int BITMAP_H = 40;
const int BITMAP_X = (128 - BITMAP_W) / 2; // = 29
const int BITMAP_Y = (64 - BITMAP_H) / 2;  // = 12

const uint8_t *bitmaps[] = {
    config_mode_bitmap,
    config_difficulty_bitmap,
    config_length_bitmap,
    config_audio_bitmap};

const char *labels[] = {
    "Game Mode",
    "Difficulty",
    "Snake Len",
    "Game Audio"};
const char *mode_labels[] = {
    "Normal Mode",
    "Wrap Mode"};
const char *difficulty_labels[] = {
    "Normal",
    "Hard"};
const char *snakelen_labels[] = {
    "increase",
    "decrease",
};
const char *audio_labels[] = {
    "yes",
    "no",
};

#define CONFIG_COUNT 4

void updateMenuAnimation()
{
    if (!menuConfig.animating)
        return;

    int speed = 8; // tốc độ mượt
    menuConfig.animOffsetX += menuConfig.animDir * speed;

    // Khi hoàn tất (đã trượt hết khung VIEW_W)
    if (menuConfig.animOffsetX >= BITMAP_W || menuConfig.animOffsetX <= -BITMAP_W)
    {
        menuConfig.currentMenu = menuConfig.targetMenu;
        menuConfig.animOffsetX = 0;
        menuConfig.animating = false;
    }
}

void drawMenuItem(int x, int y, const char *label)
{
    view_render.setCursor(x, y);
    view_render.setTextSize(1);
    view_render.print(label);
}
void renderArrowRight(int x, int y)
{
    if (menuConfig.isBlink)
        view_render.fillTriangle(x, y - 5, x, y + 5, x + 6, y, WHITE);
    else
        view_render.fillTriangle(x, y - 5, x, y + 5, x + 6, y, BLACK);
}

void renderArrowLeft(int x, int y)
{

    if (menuConfig.isBlink)
        view_render.fillTriangle(x, y, x + 6, y - 5, x + 6, y + 5, WHITE);
    else
        view_render.fillTriangle(x, y, x + 6, y - 5, x + 6, y + 5, BLACK);
}
void drawMenu()
{

    int xCenter = (128 - BITMAP_W) / 2;
    int yCenter = (64 - BITMAP_H) / 2;

    if (!menuConfig.animating)
    {
        view_render.drawBitmap(xCenter, yCenter, bitmaps[menuConfig.currentMenu], BITMAP_W, BITMAP_H, WHITE);
        switch (menuConfig.currentMenu)
        {
        case 0:
            drawMenuItem(35, 50, labels[0]);

            break;
        case 1:
            drawMenuItem(35, 50, labels[1]);

            break;
        case 2:
            drawMenuItem(35, 50, labels[2]);
            break;
        case 3:
            drawMenuItem(35, 50, labels[3]);
            break;

        default:
            break;
        }

        return;
    }

    int xOld = xCenter - menuConfig.animOffsetX;
    int xNew = xCenter + (menuConfig.animDir * BITMAP_W) - menuConfig.animOffsetX;

    view_render.drawBitmap(xOld, yCenter, bitmaps[menuConfig.currentMenu], BITMAP_W, BITMAP_H, WHITE);
    drawMenuItem(xOld + 7, 50, labels[menuConfig.currentMenu]);

    view_render.drawBitmap(xNew, yCenter, bitmaps[menuConfig.targetMenu], BITMAP_W, BITMAP_H, WHITE);
    drawMenuItem(xNew + 7, 50, labels[menuConfig.targetMenu]);

    // //clear rest
    view_render.fillRect(0, 0, 29, 64, BLACK);
    view_render.fillRect(99, 0, 128 - 99, 64, BLACK);
    view_render.drawRect(0, 0, 128, 64, WHITE);
}
bool checkNav()
{
    if (!menuConfig.ignoreNav)
        return false;
    menuConfig.ignoreNav = false;
    return true;
}
void renderConfig()
{
    updateMenuAnimation();
    drawMenu();
    renderArrowRight(118, 32);
    renderArrowLeft(10, 32);
}
void renderSubConfig()
{
    uint8_t len = strlen(labels[menuConfig.currentMenu]);
    uint8_t textWidth = len * 6;
    uint8_t x = (128 - textWidth) / 2;
    drawMenuItem(x, 10, labels[menuConfig.currentMenu]);
    uint8_t item_x = 20;
    uint8_t item_y = 25;

    for (int i = 0; i < menuConfig.labelCount; i++)
    {

        drawMenuItem(item_x, item_y, menuConfig.currentLabels[i]);
        item_y += 15;
    }
    if (menuConfig.currentMenu == 2)
    {
        view_render.setTextSize(2);
        char buf[8];
        view_render.setCursor(80, 30);
        sprintf(buf, "%02d", game.snakeGetLength());
        view_render.print(buf);
    }
    if (menuConfig.currentItem == 0)
        renderArrowRight(5, 28);
    else if (menuConfig.currentItem == 1)
        renderArrowRight(5, 43);
}
void initSubItems()
{
    switch (menuConfig.currentMenu)
    {
    case 0:
        if (game.gameGetMode() == GAME_MODE_NORMAL)
            menuConfig.currentItem = 0;
        else if (game.gameGetMode() == GAME_MODE_WRAP_AROUND)
            menuConfig.currentItem = 1;

        menuConfig.currentLabels = mode_labels;
        menuConfig.labelCount = sizeof(mode_labels) / sizeof(mode_labels[0]);
        break;
    case 1:
        // xprintf("speed : %d\n",game.snakeGetSpeed() );/
        if (game.snakeGetSpeed() == 120)
            menuConfig.currentItem = 0;
        else if (game.snakeGetSpeed() == 100)
            menuConfig.currentItem = 1;
        menuConfig.currentLabels = difficulty_labels;
        menuConfig.labelCount = sizeof(difficulty_labels) / sizeof(difficulty_labels[0]);
        break;
    case 2:
        menuConfig.currentLabels = snakelen_labels;
        menuConfig.labelCount = sizeof(snakelen_labels) / sizeof(snakelen_labels[0]);
        break;
    case 3:
        if (game.getIsAudio())
            menuConfig.currentItem = 0;
        else
            menuConfig.currentItem = 1;
        menuConfig.currentLabels = audio_labels;
        menuConfig.labelCount = sizeof(audio_labels) / sizeof(audio_labels[0]);
        break;

    default:
        break;
    }
    menuConfig.isPopup = true;
}
void chooseSubItem()
{
    int idx = menuConfig.currentItem;

    switch (menuConfig.currentMenu)
    {
    case 0:
    {
        if (idx == 0)
        {
            game.gameChangeMode(GAME_MODE_NORMAL);
            game.gameCfg.mode = GAME_MODE_NORMAL;
        }
        else

        {
            game.gameChangeMode(GAME_MODE_WRAP_AROUND);
            game.gameCfg.mode = GAME_MODE_WRAP_AROUND;
        }

        break;
    }

    case 1:
    {
        if (idx == 0)
        {
            game.snakeChangeSpeed(120);
            game.gameCfg.difficulty = 120;
        }
        else
        {
            game.snakeChangeSpeed(100);
            game.gameCfg.difficulty = 100;
        }

        break;
    }

    case 2:
    {
        if (idx == 0)
            game.snakeChangeLen(true);
        else
            game.snakeChangeLen(false);

        break;
    }

    case 3:
    {

        if (idx == 0)
        {
            game.setIsAudio(true);
            game.gameCfg.audio = true;
        }
        else
        {
            game.setIsAudio(false);
            game.gameCfg.audio = false;
        }

        break;
    }

    default:
        break;
    }
    if (menuConfig.currentMenu == 2)
    {

        return;
    }
    menuConfig.isPopup = false;
    game.saveConfig();
    menuConfig.isSuccess = true;
    timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_SUCCESS, 1000, TIMER_ONE_SHOT);
}
void renderSuccessNoti()
{
    const char *msg = "UPDATE SUCCESSFULLY";

    int textWidth = strlen(msg) * 6;
    int textHeight = 8;

    int x = (128 - textWidth) / 2;
    int y = (64 - textHeight) / 2;

    view_render.setTextSize(1);
    view_render.setCursor(x, y);

    view_render.fillRect(0, 0, 128, 64, BLACK);
    view_render.drawRect(0, 0, 128, 64, WHITE);
    view_render.drawRect(6, 37, 113, 2, WHITE);
    view_render.print(msg);
}
void view_scr_config()
{
    view_render.fillRect(0, 0, 128, 64, BLACK);
    view_render.drawRect(0, 0, 128, 64, WHITE);

    if (menuConfig.isSuccess)
    {
        renderSuccessNoti();
        return;
    }
    else if (!menuConfig.isPopup)
    {
        renderConfig();
        return;
    }
    renderSubConfig();
}

void scr_config_handle(ak_msg_t *msg)
{
    switch (msg->sig)
    {
    case SCREEN_ENTRY:
    {
        APP_DBG_SIG("SCREEN_ENTRY\n");
        menuConfig.animating = false;
        menuConfig.ignoreNav = true;
        menuConfig.animOffsetX = 0;
        menuConfig.currentMenu = 0;

        timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_UPDATE, 100, TIMER_PERIODIC);
    }
    break;

    case AC_DISPLAY_BUTON_MODE_RELEASED:
    {
        if (checkNav())
            return;
        if (game.getIsAudio())
            BUZZER_PlayTones(tones_menu_select);

        if (!menuConfig.isPopup)
        {

            initSubItems();
            return;
        }

        chooseSubItem();
    }
    break;

    case AC_DISPLAY_BUTON_UP_PRESS:
    {

        if (!menuConfig.animating && !menuConfig.isPopup)
        {

            if (game.getIsAudio())
                BUZZER_PlayTones(tones_menu_click);
            menuConfig.targetMenu = (menuConfig.currentMenu + 1) % CONFIG_COUNT;
            menuConfig.animDir = 1; // phải
            menuConfig.animating = true;
        }
        else if (menuConfig.isPopup)
        {
            if (game.getIsAudio())
                BUZZER_PlayTones(tones_menu_click);

            menuConfig.currentItem = (menuConfig.currentItem + 1) % menuConfig.labelCount;
        }
    }
    break;

    case AC_DISPLAY_BUTON_DOWN_PRESS:
    {

        if (!menuConfig.animating && !menuConfig.isPopup)
        {
            if (game.getIsAudio())
                BUZZER_PlayTones(tones_menu_click);
            menuConfig.targetMenu = (menuConfig.currentMenu - 1 + CONFIG_COUNT) % CONFIG_COUNT;
            menuConfig.animDir = -1; // trái
            menuConfig.animating = true;
        }
        else if (menuConfig.isPopup)
        {
            if (game.getIsAudio())
                BUZZER_PlayTones(tones_menu_click);
            menuConfig.currentItem = (menuConfig.currentItem - 1 + menuConfig.labelCount) % menuConfig.labelCount;
        }
    }
    break;
    case AC_DISPLAY_BUTON_MODE_LONG_RELEASED:
    {

        // APP_DBG(" ENTER MENU\n");
        if (!menuConfig.isPopup)
        {
            if (game.getIsAudio())
                BUZZER_PlayTones(tones_menu_select);

            timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_UPDATE);

            SCREEN_TRAN(scr_menu_handle, &scr_menu);
            return;
        }
        if (menuConfig.currentMenu == 2)
        {
            if (game.getIsAudio())
                BUZZER_PlayTones(tones_menu_select);

            game.gameCfg.snakeLen = game.snakeGetLength();
            game.saveConfig();
            menuConfig.isPopup = false;
            menuConfig.ignoreNav = true;
            menuConfig.isSuccess = true;
            timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_SUCCESS, 1000, TIMER_ONE_SHOT);
        }
        else
        {
            if (game.getIsAudio())
                BUZZER_PlayTones(tones_menu_select);

            menuConfig.ignoreNav = true;
            menuConfig.isPopup = false;
        }
    }
    break;

    case AC_DISPLAY_UPDATE:
    {
        menuConfig.blinkCount++;
        if (menuConfig.blinkCount >= 5)
        {
            menuConfig.isBlink = !menuConfig.isBlink;
            menuConfig.blinkCount = 0;
        }
    }

    break;
    case AC_DISPLAY_SHOW_SUCCESS:
    {

        menuConfig.isSuccess = false;
        break;
    }
    default:
        break;
    }
}
