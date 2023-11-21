#ifndef DOOR_H
#define DOOR_H

#include "things/RealThing.h"
#include "events/SimpleMessage.h"

struct Door : public RealThing {
    Door(RealThingData tD, ThingLists tL);

    bool opened, locked;
    SimpleMessage lockedMessage;
    
    virtual int checkForCollidables(Ray incoming, int incomingLayer, CollidableType collidableType);

    void open();
    void close();
    void preview();

    virtual RealThing* copyInPlace();
};

#endif
