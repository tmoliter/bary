#include "CameraEffect.h"
#include <../include/SDL2/SDL_image.h>
#include <iostream>

using namespace std;

CameraEffect::CameraEffect(int fX, int fY, int targetId, EffectType t, int d) : Thing(fX, fY), type(t), duration(d) {
    Camera::setFocus(&x,&y);
    Thing* target = Thing::things[targetId];
    cout << target->name << endl;
    targetX = &target->x;
    targetY = &target->y;
    start = frameCount;
}

void CameraEffect::destroy() {
    Camera::c->focusX = targetX;
    Camera::c->focusY = targetY;
    Thing::destroyThing(id);
}

void CameraEffect::meat() {
    if (frameCount - duration > start) {
        destroy();
        return;
    }
    switch(type) {
        case(EffectType::shake):
            shake();
        case(EffectType::pan):
            pan();
        default:
            break;
    }
}

void CameraEffect::pan() {


    cout << x << " !! " << y << endl;
    cout << *targetX << " ## " << *targetY << endl;
    if (x == *targetX && y == *targetY) {
        destroy();
        return;
    }

    int xDiff = *targetX - x;
    int yDiff = *targetY - y;

    if(xDiff > 0)
        x = x + ((xDiff / 30) + 1);
    else if (xDiff < 0)
        x = x + ((xDiff / 30) - 1);
    if(yDiff > 0)
        y = y + ((yDiff / 30) + 1);
    else if (yDiff < 0)
        y = y + ((yDiff / 30) - 1);
}

void CameraEffect::shake() {
    int xDiff = *targetX - x;
    int yDiff = *targetY - y;

    if(xDiff > 0)
        x = x + 3;
    else
        x = x -3 ;
    if(yDiff > 0)
        y = y + 3;
    else
        y = y - 3;
}

