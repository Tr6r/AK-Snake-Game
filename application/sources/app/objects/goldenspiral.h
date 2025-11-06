#ifndef __GOLDENSPIRAL_H__
#define __GOLDENSPIRAL_H__

#include <stdint.h>
#include <stdlib.h>
#include <vector>


#define TWO_PI (6.283185307f)

class GoldenSpiral
{
public:
    float x;
    float y;
    float angle;
    float radius;
    float k;
    float speedAngle;
    int   maxLoop; // 3 vòng

public:
    GoldenSpiral(float px, float py);

    bool update();
};
#endif //__GOLDENSPIRAL_H__
