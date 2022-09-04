#include "GhostFocus.h"
#include <../include/SDL2/SDL_image.h>
#include <iostream>

using namespace std;

GhostFocus::GhostFocus(Thing *&f, int targetId, EffectType t, int d) : Thing(f->getCenter()), type(t), duration(d), focus(f) {
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
    Point tp = target->getCenter();

    // Snap to target if close enough
    if (tp.x < position.x + 3 && tp.x > position.x - 3)
        position.x = tp.x;
    if (tp.y < position.y + 3 && tp.y > position.y - 3)
        position.y = tp.y;

    // Self destruct and assign camera to target when target is reached
    if (position.x == tp.x && position.y == tp.y) {
        destroy();
        return;
    }

    int xDiff = tp.x - position.x;
    int yDiff = tp.y - position.y;
    if(xDiff > 0)
        position.x += ((xDiff / 30) + 3);
    else if (xDiff < 0)
        position.x += ((xDiff / 30) - 3);
    if(yDiff > 0)
        position.y += ((yDiff / 30) + 3);
    else if (yDiff < 0)
        position.y += ((yDiff / 30) - 3);
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

