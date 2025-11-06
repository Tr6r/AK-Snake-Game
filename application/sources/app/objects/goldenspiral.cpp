#include "goldenspiral.h"
#include "xprintf.h"
GoldenSpiral::GoldenSpiral(float cx, float cy)
{
    x = cx;
    y = cy;
    angle = 0; // radian
    radius = 0;
    k = 0.45; // tweak
    maxLoop = 3;
}

void GoldenSpiral::reset()
{
    angle = 0;
    radius = 0;
}

bool GoldenSpiral::update()
{
    angle += 0.4; // speed spin
    radius = k * angle;

    // check xong 3 vòng
    if (angle >= (TWO_PI * maxLoop))
    {
        xprintf("ss");
        return false; // end
    }
    return true;
}