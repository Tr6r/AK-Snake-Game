#include "game.h"
#include "xprintf.h"
#include "app_eeprom.h"
Game game;

Game::Game()
{
}
#pragma region Game
void Game::gameInit(uint8_t idMap)
{
    score = 0;
    gameChangeState(GAME_STATE_PLAYING);
    snakeInit();
    appleInit();
    wallsInit(idMap);
}
game_state_t Game::gameGetState()
{
    return state;
}
void Game::gameChangeState(game_state_t state)
{
    this->state = state;
}

void Game::gameChangeMode(game_mode_t mode)
{
    this->mode = mode;
}
game_mode_t Game::gameGetMode()
{
    // xprintf("mode: %d\n",mode);
    return mode;
}

bool Game::getIsAudio() { return isAudio; };
void Game::setIsAudio(bool b) { this->isAudio = b; }
void Game::gameIncreaseScore() { score += 4; }
uint16_t Game::gameGetScore() { return score; }
void Game::gameSetHighestScore(uint8_t idx,uint16_t score)
{
    highScore[idx] = score;

}
uint8_t calcChecksum(uint8_t *data, uint16_t len)
{
    uint8_t cs = 0;
    while (len--)
        cs ^= *data++;
    return cs;
}
    uint16_t Game::gameGetHighestScore(uint8_t idx)
    {
        return highScore[idx];
    }

void Game::saveConfig()
{
    // ghi struct
    xprintf("mode=%d, audio=%d, diff=%d, snakeLen=%d\n",
            gameCfg.mode,
            gameCfg.audio,
            gameCfg.difficulty,
            gameCfg.snakeLen);
    eeprom_write(EEPROM_START_ADDR, (uint8_t *)&gameCfg, sizeof(GameConfig));

    // tính checksum
    uint8_t cs = calcChecksum((uint8_t *)&gameCfg, sizeof(GameConfig));

    // ghi checksum ngay sau struct
    eeprom_write(EEPROM_START_ADDR + sizeof(GameConfig), &cs, 1);
}

bool Game::loadConfig()
{
    // đọc struct thẳng vào biến private
    eeprom_read(EEPROM_START_ADDR, (uint8_t *)&gameCfg, sizeof(GameConfig));

    // đọc checksum
    uint8_t stored = 0;
    eeprom_read(EEPROM_START_ADDR + sizeof(GameConfig), &stored, 1);

    // tính lại
    uint8_t calc = calcChecksum((uint8_t *)&gameCfg, sizeof(GameConfig));

    if (calc != stored)
    {
        xprintf("ko cos epproom\n");
        gameCfg.mode = mode = GAME_MODE_WRAP_AROUND;

        gameCfg.audio = isAudio = false;

        snake.changeSpeed(150);
        gameCfg.difficulty = 150;
        snake.changeuint8Len(3);
        gameCfg.snakeLen = 3;

        for (int i = 0; i < 4; i++)
        {
            gameCfg.highScore[i] = highScore[i] = 0;
        }
        saveConfig();
        return false; // lỗi checksum
    }
    xprintf("cos epproom\n");

    // ok nac rồi thì apply
    mode = gameCfg.mode;
    isAudio = gameCfg.audio;
    snake.changeSpeed(gameCfg.difficulty);
    snake.changeuint8Len(gameCfg.snakeLen);
    xprintf("mode=%d, audio=%d, diff=%d, snakeLen=%d\n",
            gameCfg.mode,
            gameCfg.audio,
            gameCfg.difficulty,
            gameCfg.snakeLen);

    for (int i = 0; i < 4; i++)
    {
        xprintf("map %d high Score = %d\n", i, gameCfg.highScore[i]);

        highScore[i] = gameCfg.highScore[i];
    }

    for (int i = 0; i < 4; i++)
    {
    }
    return true;
}

#pragma endregion

#pragma region Snake
void Game::snakeInit()
{
    snake.init(6, 5, 4, 2);
}
void Game::snakeUpdate()
{

    if (state == GAME_STATE_PLAYING && !ignore_update)
    {

        snake.update();
    }
    else
        ignore_update = false;
}
void Game::snakeTurnLeft()
{
    snake.turnLeft();
    snake.update();
}
void Game::snakeTurnRight()
{
    snake.turnRight();
    snake.update();
}
void Game::snakeChangeSpeed(uint8_t speed)
{
    snake.changeSpeed(speed);
}
void Game::snakeChangeLen(bool b)
{
    snake.changeLen(b);
}
void Game::snakeChangeuint8Len(uint8_t l)
{
    snake.changeuint8Len(l);
}
Direction Game::snakeGetTailDirection()
{
    return snake.getTailDirection();
}
Direction Game::snakeGetHeadDirection()
{
    return snake.getHeadDirection();
}
Direction Game::snakeGetDirection(uint16_t idx)
{
    return snake.getDirection(idx);
}
uint8_t Game::snakeGetSpeed()
{
    return snake.getSpeed();
}

void Game::snakeChangeState(snake_state_t state)
{
    snake.changeState(state);
}
snake_state_t Game::snakeGetState()
{
    return snake.getState();
}
bool Game::snakeHitsBody(Coord *coord)
{
    return snake.hitsBody(coord, false);
}
bool Game::snakeHitsWall(Coord *coord)
{
    return snake.hitsWall(coord);
}
uint16_t Game::snakeGetLength()
{
    return snake.getLength();
}
uint16_t Game::snakeGetTailIndex()
{
    return snake.getTailIndex();
}
uint16_t Game::snakeGetHeadIndex()
{
    return snake.getHeadIndex();
}
Coord &Game::snakeGetSegment(uint16_t tail_index)
{
    return snake.getSegment(tail_index);
}

#pragma endregion

#pragma region Apple
void Game::appleInit()
{
    apple.init();
}
Coord &Game::appleGetSegment()
{
    return apple.getSegment();
}
void Game::appleRespawn()
{
    apple.repawm();
}

#pragma endregion

#pragma region Obstacles

void Game::wallsInit(uint8_t idMap)
{
    // xprintf("idMap : %u",idMap);
    // if (idMap == 0) obstacles.addWallBatch(map0_walls, sizeof(map0_walls)/sizeof(map0_walls[0]));
    // else if (idMap == 1) obstacles.addWallBatch(map1_walls, sizeof(map1_walls)/sizeof(map1_walls[0]));

    obstacles.addWallBatch(maps[idMap].walls, maps[idMap].count);
}
Coord Game::wallGetSegment(uint8_t idx)
{
    return obstacles.getSegment(idx);
}
uint8_t Game::wallGetLength()
{
    return obstacles.getCount();
}
uint8_t Game::wallGetTailIndex()
{
    return obstacles.getTail();
}
#pragma endregion
