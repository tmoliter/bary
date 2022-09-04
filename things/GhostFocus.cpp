#include "GhostFocus.h"
#include <../include/SDL2/SDL_image.h>
#include <iostream>

using namespace std;

GhostFocus::GhostFocus(Thing *&focus, int targetId, EffectType t, int d) : Thing(focus->position.x, focus->position.y), type(t), duration(d) {
    focus = this;
    target = Thing::things[targetId];
    cout << target->name << endl;
    start = frameCount;
}

void GhostFocus::destroy() {
    focus = target;
    Thing::destroyThing(id);
}

void GhostFocus::meat() {
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

void GhostFocus::pan() {
    int tX = target->position.x;
    int tY = target->position.y;

    // cout << position.x << " !! " << position.y << endl;
    // cout << tX << " ## " << tY << endl;

    if (position.x == tX && position.y == tY) {
        destroy();
        return;
    }

    int xDiff = tX - position.x;
    int yDiff = tY - position.y;

    if(xDiff > 0)
        position.x = position.x + ((xDiff / 30) + 1);
    else if (xDiff < 0)
        position.x = position.x + ((xDiff / 30) - 1);
    if(yDiff > 0)
        position.y = position.y + ((yDiff / 30) + 1);
    else if (yDiff < 0)
        position.y = position.y + ((yDiff / 30) - 1);
}

void GhostFocus::shake() {
    // int xDiff = *targetX - x;
    // int yDiff = *targetY - y;

    // if(xDiff > 0)
    //     x = x + 3;
    // else
    //     x = x -3 ;
    // if(yDiff > 0)
    //     y = y + 3;
    // else
    //     y = y - 3;
}

