#include "scr_gamemap.h"
#include "game.h"
#include "buzzer.h"
using namespace std;

static void view_scr_gamemap();

view_dynamic_t dyn_view_gamemap = {
    {
        .item_type = ITEM_TYPE_DYNAMIC,
    },
    view_scr_gamemap};

view_screen_t scr_gamemap = {
    &dyn_view_gamemap,
    ITEM_NULL,
    ITEM_NULL,

    .focus_item = 0,
};
gamemap_t gameMap;
const char *map_labels[] = {
    "Map 1",
    "Map 2",
    "Map 3",
    "Map 4"};
void updateMapAnimation()
{
    if (!gameMap.animating)
        return;
    const uint8_t mapW = 87;

    int speed = 8; // tốc độ mượt
    gameMap.animOffsetX += gameMap.animDir * speed;

    // Khi hoàn tất (đã trượt hết khung VIEW_W)
    if (gameMap.animOffsetX >= mapW || gameMap.animOffsetX <= -mapW)
    {
        gameMap.currentMap = gameMap.targetMap;
        gameMap.animOffsetX = 0;
        gameMap.animating = false;
    }
}
void drawMapItem(uint8_t x, uint8_t y, const char *label)
{
    view_render.setCursor(x, y);
    view_render.setTextSize(1);
    view_render.print(label);
}
void renderMapArrowRight(int x, int y)
{
    if (gameMap.isBlink)
        view_render.fillTriangle(x, y - 5, x, y + 5, x + 6, y, WHITE);
    else
        view_render.fillTriangle(x, y - 5, x, y + 5, x + 6, y, BLACK);
}

void renderMapArrowLeft(int x, int y)
{

    if (gameMap.isBlink)
        view_render.fillTriangle(x, y, x + 6, y - 5, x + 6, y + 5, WHITE);
    else
        view_render.fillTriangle(x, y, x + 6, y - 5, x + 6, y + 5, BLACK);
}
void drawCurrentMapWalls(uint8_t xStart, uint8_t yStart)
{
    for (uint8_t i = 0; i < maps[gameMap.currentMap].count; i++)
    {
        Coord c = maps[gameMap.currentMap].walls[i];
        uint8_t px = xStart + (c.x * MAP_GRID);
        uint8_t py = yStart + (c.y * MAP_GRID);

        view_render.fillRect(px, py, MAP_GRID, MAP_GRID, WHITE);
    }
}
void drawWallsAt(uint8_t xStart, uint8_t yStart, uint8_t mapId)
{

    const Coord *arr = maps[mapId].walls;
    uint8_t count = maps[mapId].count;

    for (uint8_t i = 0; i < count; i++)
    {
        uint8_t px = xStart + arr[i].x * MAP_GRID;
        uint8_t py = yStart + arr[i].y * MAP_GRID;
        view_render.fillRect(px, py, MAP_GRID, MAP_GRID, WHITE);
    }
}
void renderHihgestScore(uint8_t x, uint8_t y)
{
    //   if (x < -60 || x > 128) return;/
    char buf[20];
    sprintf(buf, "Top:%03u", game.gameGetHighestScore(gameMap.currentMap));
    view_render.setCursor(x, y);
    view_render.setTextSize(1);
    view_render.print(buf);
}
void drawMap()
{
    const uint8_t xStart = 22; // khung chính bắt đầu ở đây
    const uint8_t yStart = 15;
    const uint8_t mapW = 84;
    const uint8_t mapH = 32;

    const uint8_t XlabelStart = 25;
    const uint8_t YlabelStart = 55;

    const uint8_t XScore = 63;
    const uint8_t YScore = 55;
    if (!gameMap.animating)
    {
        view_render.drawRect(xStart, yStart, mapW, mapH, WHITE);

        drawCurrentMapWalls(xStart, yStart - 8);
        renderHihgestScore(XScore, YScore);
        drawMapItem(XlabelStart, YlabelStart, map_labels[gameMap.currentMap]);
        return;
    }

    // Khi animating
    int xOld = xStart - gameMap.animOffsetX;
    int xNew = xStart + (gameMap.animDir * mapW) - gameMap.animOffsetX;
    int xOldLable = XlabelStart - gameMap.animOffsetX;
    int xNewLable = XlabelStart + (gameMap.animDir * mapW) - gameMap.animOffsetX;

    int xOldScore = XScore - gameMap.animOffsetX;
    int xNewScore = XScore + (gameMap.animDir * mapW) - gameMap.animOffsetX;
    // Vẽ map hiện tại
    view_render.drawRect(xOld, yStart, mapW, mapH, WHITE);

    drawMapItem(xOldLable, YlabelStart, map_labels[gameMap.currentMap]);

    renderHihgestScore(xOldScore, YScore);
    renderHihgestScore(xNewScore, YScore);

    // Vẽ map tiếp theo
    view_render.drawRect(xNew, yStart, mapW, mapH, WHITE);
    drawMapItem(xNewLable, YlabelStart, map_labels[gameMap.targetMap]);
    drawWallsAt(xOld, yStart - 8, gameMap.currentMap);
    drawWallsAt(xNew, yStart - 8, gameMap.targetMap);

    // Che 2 bên ngoài
    view_render.fillRect(0, 0, xStart, 64, BLACK);
    view_render.fillRect(xStart + mapW, 0, 128 - (xStart + mapW), 64, BLACK);
    view_render.drawRect(0, 0, 128, 64, WHITE);
}

void renderMap()
{
    updateMapAnimation();
    drawMap();
    renderMapArrowRight(115, 32);
    renderMapArrowLeft(7, 32);
}

void view_scr_gamemap()
{

    view_render.fillRect(0, 0, 128, 64, BLACK);
    view_render.drawRect(0, 0, 128, 64, WHITE);
    renderMap();
}
void scr_gamemap_handle(ak_msg_t *msg)
{
    switch (msg->sig)
    {
    case SCREEN_ENTRY:
    {
        APP_DBG_SIG("SCREEN_ENTRY\n");
        gameMap.animating = false;
        gameMap.ignoreNav = true;

        timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_UPDATE, 100, TIMER_PERIODIC);
    }
    break;

    case AC_DISPLAY_BUTON_MODE_PRESS:
    {
        // timer_remove_attr(AC_TASK_DISPLAY_ID,AC_DISPLAY_SHOW_IDLE);
        task_post_pure_msg(AC_TASK_IDLE_ID, AC_IDLE_DEL);

        timer_remove_attr(AC_TASK_DISPLAY_ID,AC_DISPLAY_UPDATE);
        game.gameChangeState(GAME_STATE_PLAYING);

        game.gameInit(gameMap.currentMap);
        SCREEN_TRAN(scr_gameplay_handle, &scr_gameplay);
    }
    break;

    case AC_DISPLAY_BUTON_UP_PRESS:
    {
        if (!gameMap.animating)
        {
            // timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_IDLE, 5000, TIMER_PERIODIC);
            task_post_pure_msg(AC_TASK_IDLE_ID, AC_IDLE_RESET);


            if (game.getIsAudio())
                BUZZER_PlayTones(tones_menu_click);
            gameMap.targetMap = (gameMap.currentMap + 1) % 4;
            gameMap.animDir = 1; // phải
            gameMap.animating = true;
        }
    }
    break;

    case AC_DISPLAY_BUTON_DOWN_PRESS:
    {
        APP_DBG_SIG("SCREEN_ENTRY\n");

        if (!gameMap.animating)
        {
            // timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_IDLE, 5000, TIMER_PERIODIC);
            task_post_pure_msg(AC_TASK_IDLE_ID, AC_IDLE_RESET);


            if (game.getIsAudio())
                BUZZER_PlayTones(tones_menu_click);
            gameMap.targetMap = (gameMap.currentMap - 1 + 4) % 4;
            gameMap.animDir = -1; // trái
            gameMap.animating = true;
        }
    }
    break;

    case AC_DISPLAY_UPDATE:
    {
        gameMap.blinkCount++;
        if (gameMap.blinkCount >= 5)
        {
            gameMap.isBlink = !gameMap.isBlink;
            gameMap.blinkCount = 0;
        }
    }

    break;

    case AC_DISPLAY_BUTON_MODE_LONG_RELEASED:
    {
    }
    break;
    default:
        break;
    }
}
