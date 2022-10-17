#ifndef DOOR_H
#define DOOR_H

#include "things/RealThing.h"
#include "events/SimpleMessage.h"

struct Door : public RealThing {
    Door(Point p);
    // ~Door();

    bool opened, locked;
    SimpleMessage lockedMessage;
    
    void open();
    void close();

    virtual int checkForCollidables(Ray incoming, int incomingLayer, CollidableType collidableType);
};

#endif
