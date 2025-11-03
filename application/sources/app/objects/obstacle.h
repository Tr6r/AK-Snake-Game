#ifndef __OBSTACLE_H__
#define __OBSTACLE_H__

#include "coord.h"
#define MAX_OBSTACLE_SEGMENTS 20

extern const uint8_t wall_bitmap[6];

typedef struct
{
    const Coord *walls;
    uint8_t count;
} MapPreset;
const Coord map0_walls[] = {

};

const Coord map1_walls[] = {
    {4, 4},
    {4, 5},
    {4, 6},
    {4, 7},
    {4, 8},
    {15, 4},
    {15, 5},
    {15, 6},
    {15, 7},
    // {21, 9}, //max
};
// map 2
const Coord map2_walls[] = {
    {2, 3},
    {3, 3},
    {4, 3},
    {5, 3},
    {6, 3},
    {7, 3},
    {8, 3},
    {19, 7},
    {18, 7},
    {17, 7},
    {16, 7},
    {15, 7},
};
const Coord map3_walls[] = {
    {3, 3},
    {4, 3},
    {5, 3},
    {3, 4},
    {3, 5},

    {14, 5},
    {15, 5},
    {16, 5},
    {17, 5},
    {17, 4},
};

class Obstacle
{

public:
    Obstacle();
    void init();
    void addWallBatch(const Coord *arr, uint8_t n);
    void wallClear();
    Coord getSegment(uint8_t n);
    uint8_t getCount();
    uint8_t getTail();

private:
    Coord obstacles[MAX_OBSTACLE_SEGMENTS];
    uint8_t obstacle_head;
    uint8_t obstacle_tail;
    uint8_t obstacle_count;
};
const MapPreset maps[4] = {
    {map0_walls, sizeof(map0_walls) / sizeof(map0_walls[0])},
    {map1_walls, sizeof(map1_walls) / sizeof(map1_walls[0])},
    {map2_walls, sizeof(map2_walls) / sizeof(map2_walls[0])},
    {map3_walls, sizeof(map3_walls) / sizeof(map3_walls[0])},
};
#endif //__OBSTACLE_H__
