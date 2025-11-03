#ifndef __APPLE_H__
#define __APPLE_H__

#include "coord.h"
extern const uint8_t apple_bitmap[6];

class Apple
{

public:
    Apple();
    void init();
    void repawm();
    //getter
    
    Coord& getSegment();
private:
    Coord coord;
};

#endif //__APPLE_H__
