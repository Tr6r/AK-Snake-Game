#include "apple.h"
#include "game.h"
#include <stdlib.h>
#include <xprintf.h>
const uint8_t apple_bitmap[6] = {
    0b001100,
    0b011110,
    0b111111,
    0b111111,
    0b011110,
    0b001100,
};
Apple::Apple()
{
}
void Apple::init()  
{
    coord.x = 0;
    coord.y = 0;
    repawm();
}

Coord &Apple::getSegment() { return coord; }

Coord getNewCoord()
{
    Coord c;
    do
    {
        c.x = rand() % FIELD_W;
        c.y = 3 + rand() % (FIELD_H - 3);
    } while (game.snakeHitsBody(&c)||game.snakeHitsWall(&c)); // đảm bảo rắn không đè
    return c;
}
void Apple::repawm()
{
    coord = getNewCoord();
}