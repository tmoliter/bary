#include "things/Door.h"

Door::Door() : RealThing(Point(0,0)), 
    opened(false),
    locked(false) {
}

int Door::checkForCollidables(Ray incoming, int incomingLayer, CollidableType collidableType) {
    if (opened)
        return 0;
    if (collidableType == CollidableType::obstruction)
        return RealThing::checkForCollidables(incoming, incomingLayer, collidableType);
    if (RealThing::checkForCollidables(incoming, incomingLayer, collidableType)) {
        if (!locked)
            open();
        return 1;
    };
    return 0;
}

void Door::open() {
    opened = true;
    sprites[1]->active = true;

    sprites[0]->active = false;
    interactables[0]->active = false;
    triggers[0]->active = false;
    obstructions[0]->active = false;
}

void Door::close() {
    opened = false;
    sprites[1]->active = false;

    sprites[0]->active = true;
}
