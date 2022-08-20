#include "Walk.h"
#include <SDL2/SDL.h>
#include <iostream>

using namespace std;

void Walk::applySpeed(int x, int y, int speed, int cycle) {
    if (speed == 0)
        return;
    if (speed < 3 && cycle % (3 - speed) != 0) {
        return;
    }
    int appliedSpeed = speed < 4 ? 1 : speed - 1;
    renderRect.x = renderRect.x + (x * appliedSpeed);
    renderRect.y = renderRect.y + (y * appliedSpeed);
};

void Walk::animate(int x, int y, int speed, int cycle) {
    int totalFrames = 7;
    int delayPerFrame = 12 / speed;
    if (x != 0 || y != 0) {
        if(y > 0)
            sourceRect.y = sourceRect.h * 0;
        else if(y < 0)
            sourceRect.y = sourceRect.h * 1;
        else if(x < 0)
            sourceRect.y = sourceRect.h * 2;
        else if(x > 0)
            sourceRect.y = sourceRect.h * 3;

        int frame = ((cycle / delayPerFrame) % totalFrames) + 1;
        sourceRect.x = frame * sourceRect.w;
    }
    else if (sourceRect.x != 0)
        sourceRect.x = 0;
}