#ifndef __GOLDENSPIRAL_H__
#define __GOLDENSPIRAL_H__

#include <stdint.h>

const uint8_t fib[] = {1, 1, 2, 3, 5, 8, 13, 21, 34};
#define FIB_COUNT (sizeof(fib) / sizeof(fib[0]))
#define TWO_PI (6.283185307f)

class GoldenSpiral
{
public:
    GoldenSpiral(float cx, float cy);
    float x;
    float y;
    float angle;
    float radius;
    float k;
    float speedAngle;
    int maxLoop; // 3 vòng
    void reset();
    bool update();
};

#endif //__GOLDENSPIRAL_H__
