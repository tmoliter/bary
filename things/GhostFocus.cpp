#include "GhostFocus.h"
#include <../include/SDL2/SDL_image.h>
#include <iostream>

using namespace std;

GhostFocus::GhostFocus(Thing *&f, string targetName) : Thing(f->getCenter()), focus(f) {
    g = this;
    focus = this;
    target = Thing::things[targetName];
}

void GhostFocus::destroy() {
    focus = target;
    GhostFocus::g = nullptr;
    Thing::destroyThing(name);
}

void GhostFocus::meat() {
    pan();
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

// STATIC

GhostFocus* GhostFocus::g = nullptr;

int GhostFocus::create(Thing *&f, string targetName) {
    if (!g) {
        new GhostFocus(f, targetName);
        return 1;
    }
    return 0;
}