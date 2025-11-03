
#include "obstacle.h"
#include "xprintf.h"
const uint8_t wall_bitmap[6] = {
    0b111111,  // row 1
    0b110001,  // row 2
    0b100011,  // row 3
    0b110001,  // row 4
    0b100011,  // row 5
    0b111111   // row 6
};
Obstacle::Obstacle()
{
    
}

void Obstacle::init()
{
    obstacle_count = 0;
    obstacle_head = 0;
    obstacle_tail = 0;
     for (uint8_t i = 0; i < MAX_OBSTACLE_SEGMENTS; i++)
    {
        obstacles[i].x = -1;
        obstacles[i].y = -1;
    }
}
void Obstacle::wallClear()
{
     obstacle_count = 0;
    obstacle_head = 0;
    obstacle_tail = 0;
}
void Obstacle::addWallBatch(const Coord *arr, uint8_t n)
{
    wallClear();
     for (uint8_t i = 0; i < n; i++)
    {
        if (obstacle_count < MAX_OBSTACLE_SEGMENTS)
        {
            obstacles[(obstacle_tail + obstacle_count) % MAX_OBSTACLE_SEGMENTS] = arr[i];
            obstacle_count++;
        }
        else
        {
            // Bộ đệm đầy → ghi đè phần tử cũ (tăng tail)
            obstacles[obstacle_tail] = arr[i];
            obstacle_tail = (obstacle_tail + 1) % MAX_OBSTACLE_SEGMENTS;
        }
    }
}
Coord Obstacle::getSegment(uint8_t idx)
{
    if (idx >= obstacle_count)
    {
        return {-1, -1};
    }
    uint8_t realIdx = (obstacle_tail + idx) % MAX_OBSTACLE_SEGMENTS;
    return obstacles[realIdx];
}

uint8_t Obstacle::getCount() { return obstacle_count; }
uint8_t Obstacle::getTail() { return obstacle_tail; }
