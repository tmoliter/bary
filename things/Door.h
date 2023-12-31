#ifndef DOOR_H
#define DOOR_H

#include "things/RealThing.h"

struct Door : public RealThing {
    Door(RealThingData tD, map<string, RealThing*>& things);

    bool opened, locked;
    string lockedMessage;
    
    virtual int checkForCollidables(Ray incoming, int incomingLayer, CollidableType collidableType);

    void open();
    void close();
    void preview();

    virtual RealThing* copyInPlace();
};

#endif
