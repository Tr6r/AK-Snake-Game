#include "snake.h"
#include "xprintf.h"
#include "game.h"
#include "buzzer.h"

const uint8_t snake_head_down[6] = {
    0b011110,
    0b111111,
    0b101101,
    0b111111,
    0b011110,
    0b001100};

const uint8_t snake_head_right[6] = {
    0b011100,
    0b110110,
    0b111111,
    0b111111,
    0b110110,
    0b011100};

const uint8_t snake_head_left[6] = {
    0b001110,
    0b011011,
    0b111111,
    0b111111,
    0b011011,
    0b001110};

const uint8_t snake_head_up[6] = {
    0b001100,
    0b011110,
    0b111111,
    0b101101,
    0b111111,
    0b011110};

const uint8_t snake_tail_up[6] = {
    0b111111,
    0b111111,
    0b011110,
    0b011110,
    0b011110,
    0b001100};
const uint8_t snake_tail_down[6] = {
    0b001100,
    0b011110,
    0b011110,
    0b011110,
    0b111111,
    0b111111};
const uint8_t snake_tail_left[6] = {
    0b000011,
    0b011111,
    0b111111,
    0b111111,
    0b011111,
    0b000011};

const uint8_t snake_tail_right[6] = {
    0b110000,
    0b111110,
    0b111111,
    0b111111,
    0b111110,
    0b110000};
const uint8_t snake_body_bitmap[6] = {
    0b011110,
    0b111011,
    0b110111,
    0b111101,
    0b101111,
    0b011110};
Snake::Snake()
{
}

void Snake::init(int16_t startX, int16_t startY, uint16_t initLen, uint8_t spd)
{
    if (game.getIsAudio())
        BUZZER_PlayTones(tones_snake_init);
    next_dir = Direction::RIGHT;
    eat = false;
    length = game.snakeGetLength();
    snake_temp_len = length;
    alive = true;
    int16_t x = startX;
    int16_t y = startY;
    int8_t dir_h = 1; // bắt đầu thân đi từ phải qua trái (vì đầu ở bên phải)

    // // tạo thân từ đuôi → đầu
    for (uint16_t i = 0; i < length; ++i)
    {
        uint16_t idx = i % MAX_SNAKE_LEN;
        body[idx] = {x, y};
        head_index = idx;

        x += dir_h;
    }

    //     // nếu tới biên => gấp khúc xuống hàng và đảo hướng
    //     if (x < 0)
    //     {
    //         x = 0;
    //         y++;
    //         dir_h = 1;
    //     }
    //     else if (x >= FIELD_W)
    //     {
    //         x = FIELD_W - 1;
    //         y++;
    //         dir_h = -1;
    //     }

    //     if (y >= FIELD_H)
    //         break;
    // }

    tail_index = 0;
}

void Snake::changeDirection(Direction newDir)
{
    if ((dir == Direction::UP && newDir == Direction::DOWN) ||
        (dir == Direction::DOWN && newDir == Direction::UP) ||
        (dir == Direction::LEFT && newDir == Direction::RIGHT) ||
        (dir == Direction::RIGHT && newDir == Direction::LEFT))
        return;

    next_dir = newDir;
}
void Snake::changeSpeed(uint8_t speed)
{
    this->speed = speed;
}
void Snake::changeLen(bool b)
{
    if (b && length <= MAX_SNAKE_LEN)
    {
        snake_temp_len++;
        length++;
    }
    else if (length > 2)
    {
        snake_temp_len--;

        length--;
    }
}
void Snake::changeuint8Len(uint8_t l)
{
    if (l >= MAX_SNAKE_LEN)
    {
        length = MAX_SNAKE_LEN;
    }
    else if (length < 2)
    {
        length = 2;
    }
    else length = l;
}

Coord Snake::dirToDelta(Direction d)
{
    switch (d)
    {
    case Direction::UP:
        return {0, -1};
    case Direction::DOWN:
        return {0, 1};
    case Direction::LEFT:
        return {-1, 0};
    case Direction::RIGHT:
        return {1, 0};
    default:
        return {0, 0};
    }
}
void Snake::changeState(snake_state_t state)
{
    this->state = state;
}

Coord Snake::moveSnakeWrapAround(Coord *delta)
{
    Coord newHead;
    newHead.x = body[head_index].x + delta->x;
    newHead.y = body[head_index].y + delta->y;
    switch (dir)
    {
    case Direction::UP:
        newHead.y--;
        break;
    case Direction::DOWN:
        newHead.y++;
        break;
    case Direction::LEFT:
        newHead.x--;
        break;
    case Direction::RIGHT:
        newHead.x++;
        break;
    }
    // --- Xử lý wrap-around ---
    if (newHead.x < 0)
        newHead.x = FIELD_W - 1;
    else if (newHead.x >= FIELD_W)
        newHead.x = 0;

    if (newHead.y < 2)
        newHead.y = FIELD_H - 1;
    else if (newHead.y >= FIELD_H)
        newHead.y = 2;
    return newHead;
}
Coord Snake::moveSnake(Coord *delta)
{
    Coord newHead;
    newHead.x = body[head_index].x + delta->x;
    newHead.y = body[head_index].y + delta->y;
    return newHead;
}

void Snake::update()
{
    if (!alive || getState() == SNAKE_STATE_TURNING)
        return;
    if (game.getIsAudio())
        BUZZER_PlayTones(tones_snake_move);

    if (dir != next_dir)
    {
        if (game.getIsAudio())
            BUZZER_PlayTones(tones_snake_turn);
        dir = next_dir;
    }
    Coord delta = dirToDelta(dir);
    Coord newHead = moveSnake(&delta);

    if (hitsWall(&newHead))
    {
        alive = false;
        length = snake_temp_len;
        if (game.getIsAudio())
            BUZZER_PlayTones(tones_snake_crash);

        game.gameChangeState(GAME_STATE_GAMEOVER);
        return;
    }

    if (hitsEdge(&newHead))
    {
        if (game.gameGetMode() == GAME_MODE_NORMAL)
        {

            alive = false;
            if (game.getIsAudio())
                BUZZER_PlayTones(tones_snake_crash);

            length = snake_temp_len;

            game.gameChangeState(GAME_STATE_GAMEOVER);
            return;
        }

        newHead = moveSnakeWrapAround(&delta);
    }

    bool ignore_tail = !eat;
    if (hitsBody(&newHead, ignore_tail))
    {
        alive = false;
        length = snake_temp_len;
        if (game.getIsAudio())
            BUZZER_PlayTones(tones_snake_crash);

        game.gameChangeState(GAME_STATE_GAMEOVER);
        return;
    }

    if (hitsBody(&game.appleGetSegment()))
    {
        if (game.getIsAudio())
            BUZZER_PlayTones(tones_snake_eat);

        eat = true;
    }

    head_index = (head_index + 1) % MAX_SNAKE_LEN;
    body[head_index] = newHead;

    if (!eat)
    {
        tail_index = (tail_index + 1) % MAX_SNAKE_LEN;
    }
    else
    {
        if (length < MAX_SNAKE_LEN)
            length++;
        game.gameIncreaseScore();
        game.appleRespawn();
        eat = false;
    }
    // game.snakeChangeState(SNAKE_STATE_MOVING);
}

bool Snake::isAlive() { return alive; }
uint16_t Snake::getLength() { return length; }
uint16_t Snake::getTailIndex() { return tail_index; }
uint16_t Snake::getHeadIndex() { return head_index; }
Coord &Snake::getSegment(uint16_t idx) { return body[idx]; }
Direction Snake::getHeadDirection()
{
    return dir; // dir là hướng hiện tại của rắn
}
Direction Snake::getDirection(uint16_t idx)
{
    if (length <= 1)
        return dir; // Rắn 1 đoạn thì lấy hướng hiện tại luôn

    // Nếu là đuôi thì dùng next segment để xác định
    if (idx == tail_index)
    {
        uint16_t nextIdx = (tail_index + 1) % MAX_SNAKE_LEN;
        Coord &tail = body[tail_index];
        Coord &next = body[nextIdx];
        if (next.x > tail.x)
            return Direction::RIGHT;
        if (next.x < tail.x)
            return Direction::LEFT;
        if (next.y > tail.y)
            return Direction::DOWN;
        return Direction::UP;
    }

    // Nếu là đầu thì lấy hướng hiện tại
    if (idx == head_index)
        return dir;

    // Các segment giữa → xác định theo hướng nối 2 ô liền kề
    uint16_t nextIdx = (idx + 1) % MAX_SNAKE_LEN;
    Coord &cur = body[idx];
    Coord &next = body[nextIdx];

    if (next.x > cur.x)
        return Direction::RIGHT;
    if (next.x < cur.x)
        return Direction::LEFT;
    if (next.y > cur.y)
        return Direction::DOWN;
    return Direction::UP;
}

Direction Snake::getTailDirection()
{
    if (length < 2)
        return dir; // Nếu rắn ngắn, coi giống head

    uint16_t nextIdx = (tail_index + 1) % MAX_SNAKE_LEN;
    Coord &tail = body[tail_index];
    Coord &next = body[nextIdx];

    if (next.x > tail.x)
        return Direction::LEFT;
    if (next.x < tail.x)
        return Direction::RIGHT;
    if (next.y > tail.y)
        return Direction::DOWN;
    return Direction::UP;
}
void Snake::turnRight()
{

    switch (dir)
    {
    case Direction::UP:
        next_dir = Direction::LEFT;
        break;
    case Direction::RIGHT:
        next_dir = Direction::UP;
        break;
    case Direction::DOWN:
        next_dir = Direction::RIGHT;
        break;
    case Direction::LEFT:
        next_dir = Direction::DOWN;
        break;
    default:
        break;
    }
}

snake_state_t Snake::getState() { return state; }
uint8_t Snake::getSpeed() { return speed; }
void Snake::turnLeft()
{

    switch (dir)
    {
    case Direction::UP:
        next_dir = Direction::RIGHT;
        break;
    case Direction::RIGHT:
        next_dir = Direction::DOWN;
        break;
    case Direction::DOWN:
        next_dir = Direction::LEFT;
        break;
    case Direction::LEFT:
        next_dir = Direction::UP;
        break;
    default:
        break;
    }
}

bool Snake::hitsBody(Coord *p, bool ignoreTail)
{
    for (uint16_t i = 0; i < length; ++i)
    {
        uint16_t idx = (tail_index + i) % MAX_SNAKE_LEN;

        // Bỏ qua đuôi nếu cần
        if (ignoreTail && i == 0)
            continue;

        // So sánh vị trí
        if (body[idx].x == p->x && body[idx].y == p->y)
        {
            return true;
        }
    }
    return false;
}
bool Snake::hitsEdge(Coord *p)
{
    if (p->x < 0 || p->x >= FIELD_W ||
        p->y < 2 || p->y >= FIELD_H)
    {
        return true;
    }
    return false;
}
bool Snake::hitsWall(Coord *p)
{

    uint8_t idx = game.wallGetTailIndex();
    for (uint8_t i = 0; i < game.wallGetLength(); i++)
    {
        Coord c = game.wallGetSegment(idx);

        if (c.x == p->x && c.y == p->y)
            return true;

        idx = (idx + 1) % MAX_OBSTACLE_SEGMENTS;
    }
    return false;
}