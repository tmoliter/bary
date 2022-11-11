#ifndef DOOR_H
#define DOOR_H

#include "things/RealThing.h"
#include "events/SimpleMessage.h"

struct Door : public RealThing {
    Door(Point p);

    bool opened, locked;
    SimpleMessage lockedMessage;
    
    virtual int checkForCollidables(Ray incoming, int incomingLayer, CollidableType collidableType);

    void open();
    void close();
    void preview();

    virtual Thing* copyInPlace();
};

#endif
