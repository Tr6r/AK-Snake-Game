#include "scr_gameplay.h"
#include "game.h"

#include <functional>
using namespace std;

// --- Dynamic view ---
static void view_scr_gameplay();
gameplay_t gameplay;
view_dynamic_t dyn_view_gameplay = {
    {.item_type = ITEM_TYPE_DYNAMIC},
    view_scr_gameplay};

view_screen_t scr_gameplay = {
    &dyn_view_gameplay,
    ITEM_NULL,
    ITEM_NULL,
    .focus_item = 0,
};
void drawSegmentAt(int16_t x, int16_t y, const uint8_t bitmap[6])
{
    for (int row = 0; row < 6; row++)
    {
        uint8_t bits = bitmap[row];
        for (int col = 0; col < 6; col++)
        {
            bool pixelOn = bits & (1 << (5 - col));
            view_render.fillRect(x + col, y + row, 1, 1, pixelOn ? WHITE : BLACK);
        }
    }
}
void renderSnake()
{
    uint16_t headIdx = game.snakeGetHeadIndex();
    uint16_t tailIdx = game.snakeGetTailIndex();
    Direction headDir = game.snakeGetHeadDirection();
    Direction tailDir = game.snakeGetTailDirection();
    // Vẽ thân (trừ head và tail)
    uint16_t idx = (tailIdx + 1) % MAX_SNAKE_LEN;
    for (uint16_t i = 1; i < game.snakeGetLength() - 1; ++i)
    {
        Coord &c = game.snakeGetSegment(idx);
        int px = c.x * CELL_W;
        int py = c.y * CELL_H;

        for (int j = 0; j < 6; ++j)
        {
            for (int i = 0; i < 6; ++i)
            {
                if (snake_body_bitmap[j] & (1 << (5 - i)))
                {
                    view_render.drawPixel(px + i, py + j, WHITE);
                }
            }
        }
        idx = (idx + 1) % MAX_SNAKE_LEN;
    }
    // --- Vẽ đầu rắn ---
    {
        Coord head = game.snakeGetSegment(headIdx);
        const uint8_t *bmp = nullptr;
        switch (headDir)
        {
        case Direction::UP:
            bmp = snake_head_up;
            break;
        case Direction::DOWN:
            bmp = snake_head_down;
            break;
        case Direction::LEFT:
            bmp = snake_head_left;
            break;
        case Direction::RIGHT:
            bmp = snake_head_right;
            break;
        }
        if (bmp)
        {
            for (uint8_t y = 0; y < CELL_H; ++y)
            {
                uint8_t row = bmp[y];
                for (uint8_t x = 0; x < CELL_W; ++x)
                {
                    bool pixel = (row >> (CELL_W - 1 - x)) & 1;
                    if (pixel)
                        view_render.drawPixel(head.x * CELL_W + x, head.y * CELL_H + y, WHITE);
                }
            }
        }
    }
    // --- Vẽ đuôi rắn ---
    {
        Coord tail = game.snakeGetSegment(tailIdx);
        const uint8_t *bmp = nullptr;
        switch (tailDir)
        {
        case Direction::UP:
            bmp = snake_tail_up;
            break;
        case Direction::DOWN:
            bmp = snake_tail_down;
            break;
        case Direction::LEFT:
            bmp = snake_tail_left;
            break;
        case Direction::RIGHT:
            bmp = snake_tail_right;
            break;
        }
        if (bmp)
        {
            for (uint8_t y = 0; y < CELL_H; ++y)
            {
                uint8_t row = bmp[y];
                for (uint8_t x = 0; x < CELL_W; ++x)
                {
                    bool pixel = (row >> (CELL_W - 1 - x)) & 1;
                    if (pixel)
                        view_render.drawPixel(tail.x * CELL_W + x, tail.y * CELL_H + y, WHITE);
                }
            }
        }
    }
}
void checkHighestScore()
{
    const uint16_t newScore = game.gameGetScore();

    const uint8_t idx = gameMap.currentMap;
    const uint16_t topScore = game.gameCfg.highScore[idx];

    if (newScore > topScore)
    {
        game.gameCfg.highScore[idx] = newScore;
        game.gameSetHighestScore(idx, newScore);
        game.saveConfig();
    }
}
void renderApple()
{
    if (gameplay.apple_blink_state)
    {
        Coord &apple_c = game.appleGetSegment();

        int px = apple_c.x * CELL_W;
        int py = apple_c.y * CELL_H;

        for (int j = 0; j < 6; ++j)
        {
            for (int i = 0; i < 6; ++i)
            {
                if (apple_bitmap[j] & (1 << (5 - i)))
                {
                    view_render.drawPixel(px + i, py + j, WHITE);
                }
            }
        }
    }
}
void renderWalls()
{
    uint8_t wall_idx = game.wallGetTailIndex();

    for (uint16_t i = 0; i < game.wallGetLength(); i++)
    {
        Coord wall_c = game.wallGetSegment(wall_idx);
        if (wall_c.x <= 0 || wall_c.y <= 0)
        {
            wall_idx = (wall_idx + 1) % MAX_OBSTACLE_SEGMENTS;
            continue;
        }
        int px = wall_c.x * CELL_W;
        int py = wall_c.y * CELL_H;

        for (int j = 0; j < 6; ++j)
        {
            for (int i = 0; i < 6; ++i)
            {
                if (wall_bitmap[j] & (1 << (5 - i)))
                {
                    view_render.drawPixel(px + i, py + j, WHITE);
                }
            }
        }

        wall_idx = (wall_idx + 1) % MAX_OBSTACLE_SEGMENTS;
    }
}
void renderGameOver()
{

    timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_SNAKE_UPDATE);

    checkHighestScore();

    view_render.setTextSize(2);
    view_render.setTextColor(WHITE);
    view_render.setCursor(11, 23);
    view_render.print("GAME OVER");
    view_render.fillRect(11, 39, 107, 1, WHITE);

    view_render.setTextSize(1);
    view_render.setCursor(70, 45);
    char buf[16];

    sprintf(buf, "SCORE:%02d", game.gameGetScore());
    view_render.print(buf);
    view_render.fillRect(70, 54, 47, 1, WHITE);
}
void renderGameInfo()

{
    char buf[16];

    // len
    view_render.setCursor(1, 3);
    sprintf(buf, "len:%02d", game.snakeGetLength());
    view_render.print(buf);

    // score
    view_render.setCursor(40, 3);
    sprintf(buf, "score:%02d", game.gameGetScore());
    view_render.print(buf);

    // gameplay.fps
    view_render.setCursor(90, 3);
    sprintf(buf, "fps:%02d", gameplay.fps);
    view_render.print(buf);
}

// --- Render ---
void view_scr_gameplay()
{
    view_render.fillRect(0, 0, 128, 64, BLACK);
    view_render.drawRect(0, 12, 128, 48, WHITE);
    view_render.setTextSize(1);
    renderGameInfo();

    if (game.gameGetState() == GAME_STATE_GAMEOVER)
    {
        task_post_pure_msg(AC_TASK_IDLE_ID, AC_IDLE_INIT);
        renderGameOver();
    }
    else
    {

        renderSnake();

        renderApple();
        renderWalls();
    }
}

// --- Handle ---
void scr_gameplay_handle(ak_msg_t *msg)
{
    switch (msg->sig)
    {
    case SCREEN_ENTRY:
        APP_DBG_SIG("SCREEN_ENTRY (gamelay)\n");
        timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_SNAKE_UPDATE, game.snakeGetSpeed(), TIMER_PERIODIC);
        break;

    case AC_DISPLAY_BUTON_MODE_PRESS:
        if (game.gameGetState() == GAME_STATE_PLAYING)
        {
            game.gameChangeState(GAME_STATE_PAUSE);
            timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_SNAKE_UPDATE);
        }
        else if (game.gameGetState() == GAME_STATE_GAMEOVER)
        {
            game.gameChangeState(GAME_STATE_MENU);
            SCREEN_TRAN(scr_menu_handle, &scr_menu);
        }
        else
        {
            game.gameChangeState(GAME_STATE_PLAYING);
            timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_SNAKE_UPDATE, game.snakeGetSpeed(), TIMER_PERIODIC);
        }

        break;
    case AC_DISPLAY_SNAKE_UPDATE:
        gameplay.apple_blink_count++;
        gameplay.frame_count++;
        gameplay.fps_timer += 250;
        if (gameplay.apple_blink_count >= 3)
        {
            gameplay.apple_blink_state = !gameplay.apple_blink_state;
            gameplay.apple_blink_count = 0;
        }
        if (gameplay.fps_timer >= 1000)
        {
            gameplay.fps = gameplay.frame_count;
            gameplay.frame_count = 0;
            gameplay.fps_timer = 0;
        }
        game.snakeUpdate();

        break;

    default:
        break;
    }
}
