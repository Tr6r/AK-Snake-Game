#ifndef __GAME_H__
#define __GAME_H__

#include "coord.h"

#include "snake.h"
#include "apple.h"
#include "obstacle.h"

enum game_state_t
{
    GAME_STATE_MENU,
    GAME_STATE_SETTING,
    GAME_STATE_RANK,
    GAME_STATE_PLAYING,
    GAME_STATE_PAUSE,
    GAME_STATE_GAMEOVER,
};
enum game_mode_t
{
    GAME_MODE_NORMAL,
    GAME_MODE_WRAP_AROUND,

};
struct GameConfig {
    game_mode_t mode;
    uint8_t difficulty;
    uint8_t snakeLen;
    bool audio;

    uint16_t highScore[4]; // 4 map
};
class Game
{
public:
    bool ignore_update = false;
    GameConfig gameCfg;
    uint16_t highScore[4];
    // game
    Game();
    void gameInit(uint8_t idMap);
    void gameChangeState(game_state_t state);
    void gameIncreaseScore();
    void gameChangeMode(game_mode_t mode);
    void setIsAudio(bool b);
    void saveConfig();
    bool loadConfig();
    void gameSetHighestScore(uint8_t idx,uint16_t score);

    // snake
    void snakeInit();
    void snakeUpdate();
    void snakeTurnLeft();
    void snakeTurnRight();
    bool snakeHitsBody(Coord *c);
    bool snakeHitsWall(Coord *c);
    void snakeChangeState(snake_state_t state);
    void snakeChangeSpeed(uint8_t speed);
    void snakeChangeLen(bool b);
    void snakeChangeuint8Len(uint8_t l);
    // apple
    void appleInit();
    void appleRespawn();
    // obstacle
    void wallsInit(uint8_t idMap);
    // GETTER
    // game
    game_state_t gameGetState();
    game_mode_t gameGetMode();
    uint16_t gameGetScore();
    bool getIsAudio();
    uint16_t gameGetHighestScore(uint8_t idx);
    // snake
    uint16_t snakeGetLength();
    uint8_t snakeGetSpeed();
    uint16_t snakeGetTailIndex();
    uint16_t snakeGetHeadIndex();
    Coord &snakeGetSegment(uint16_t tail_index);
    snake_state_t snakeGetState();
    Direction snakeGetHeadDirection();
    Direction snakeGetTailDirection();
    Direction snakeGetDirection(uint16_t idx);

    /// apple
    Coord &appleGetSegment();
    // obstacles
    Coord wallGetSegment(uint8_t idx);
    uint8_t wallGetLength();
    uint8_t wallGetTailIndex();

private:
    uint16_t score;
    game_state_t state;
    Snake snake;
    Apple apple;
    Obstacle obstacles;
    game_mode_t mode;
    bool isAudio;
    

};
extern Game game;
#endif //__GAME_H__
