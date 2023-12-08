#include "things/Door.h"

Door::Door(RealThingData tD, ThingLists tL) : RealThing(tD, tL), 
    opened(false),
    locked(false) {
}

// This neds to be reworked, perhaps with Lua events
int Door::checkForCollidables(Ray incoming, int incomingLayer, CollidableType collidableType) {
    if (opened)
        return 0;
    if (locked && collidableType == CollidableType::obstruction)
        return RealThing::checkForCollidables(incoming, incomingLayer, collidableType);
    if (RealThing::checkForCollidables(incoming, incomingLayer, collidableType)) {
        if (!locked)
            open();
        // Fire off phrase event?
        return 1;
    };
    return 0;
}

void Door::open() {
    opened = true;
    sprites[1]->active = true;
    sprites[0]->active = false;
    interactables.begin()->second->active = false;
    triggers.begin()->second->active = false;
    obstructions.begin()->second->active = false;
}

void Door::close() {
    opened = false;
    sprites[1]->active = false;
    sprites[0]->active = true;
}

void Door::preview() {
    opened = true;
    sprites[1]->active = true;
    sprites[0]->active = true;
    interactables.begin()->second->active = true;
    triggers.begin()->second->active = true;
    obstructions.begin()->second->active = true;
    removeHighlight();
}

RealThing* Door::copyInPlace() {
    return new Door(*this);
}