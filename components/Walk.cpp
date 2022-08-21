#include "Walk.h"
#include <SDL2/SDL.h>
#include <iostream>

using namespace std;

void Walk::move(int xD, int yD, int &x, int &y, int tick){
    if (speed == 0)
        return;
    if (speed < 3 && tick % (3 - speed) != 0) {
        return;
    }
    int appliedSpeed = speed < 4 ? 1 : speed;
    x = x + (xD * appliedSpeed);
    y = y + (yD * appliedSpeed);
};

void Walk::animate(int xD, int yD, int tick) {
    int totalFrames = 7;
    int delayPerFrame = 12 / speed;
    if (xD != 0 || yD != 0) {
        if(yD > 0)
            sourceRect.y = sourceRect.h * 0;
        else if(yD < 0)
            sourceRect.y = sourceRect.h * 1;
        else if(xD < 0)
            sourceRect.y = sourceRect.h * 2;
        else if(xD > 0)
            sourceRect.y = sourceRect.h * 3;

        int frame = ((tick / delayPerFrame) % totalFrames) + 1;
        sourceRect.x = frame * sourceRect.w;
    }
    else if (sourceRect.x != 0)
        sourceRect.x = 0;
}

void Walk::changeSpeed(bool decrease) {
    if (decrease && speed > 0) {
        speed -= 1;
        return;
    }
    speed +=1;
}