#include "goldenspiral.h"
#include "xprintf.h"
GoldenSpiral::GoldenSpiral(float px, float py)
{
    x = px;
    y = py;
    angle = 0;
    radius = 0;
    k = 0.5;
    speedAngle = 0.3;
    maxLoop = 3;
}

bool GoldenSpiral::update()
{
    angle += speedAngle;
    radius = k * angle;

    if (angle >= (TWO_PI * maxLoop))
    {
        return false;
    }
    return true;
}
