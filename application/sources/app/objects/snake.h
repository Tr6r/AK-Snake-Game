#ifndef __SNAKE_H__
#define __SNAKE_H__

#include <cstdint> // dùng cho C++
#include "coord.h"

#define MAX_SNAKE_LEN 50
#define FIELD_W 21
#define FIELD_H 10
#define CELL_W 6
#define CELL_H 6

enum class Direction : uint8_t
{
    UP,
    RIGHT,
    DOWN,
    LEFT
};

enum snake_state_t
{
    SNAKE_STATE_MOVING,
    SNAKE_STATE_STOP,
    SNAKE_STATE_TURNING,
};

extern const uint8_t snake_head_down[6];
extern const uint8_t snake_head_right[6];
extern const uint8_t snake_head_left[6];
extern const uint8_t snake_head_up[6];

extern const uint8_t snake_tail_down[6];
extern const uint8_t snake_tail_right[6];
extern const uint8_t snake_tail_left[6];
extern const uint8_t snake_tail_up[6];

extern const uint8_t snake_body_bitmap[6];
class Snake
{
public:
    Snake();
    float moveOffset; 
    void init(int16_t startX, int16_t startY, uint16_t initLen, uint8_t spd);
    void update();
    Coord dirToDelta(Direction d);
    void changeDirection(Direction newDir);
    void changeSpeed(uint8_t speed);
    void changeLen(bool b);
    bool hitsBody(Coord *pos, bool ignore_tail = true);
    void turnLeft();
    void turnRight();
    void changeState(snake_state_t state);
    bool hitsWall(Coord *p);
    bool hitsEdge(Coord *p);
    Coord moveSnake(Coord *delta);
    Coord moveSnakeWrapAround(Coord *delta);
void changeuint8Len(uint8_t l);

    // Getter
    bool isAlive();
    Direction getHeadDirection();
    Direction getDirection(uint16_t idx);

    Direction getTailDirection();
    uint16_t getLength();
    uint8_t getSpeed();
    Coord &getSegment(uint16_t idx);
    uint16_t getTailIndex();
    uint16_t getHeadIndex();
    snake_state_t getState();

private:
    Coord body[MAX_SNAKE_LEN];
    uint16_t head_index;
    uint16_t tail_index;
    uint16_t length=3;
    uint16_t snake_temp_len;

    uint32_t speed = 120;

    snake_state_t state;

    Direction next_dir;
    Direction dir;
    bool alive;
    bool eat;
};

#endif //__SNAKE_H__
