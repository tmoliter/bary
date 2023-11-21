#ifndef FIELD_PLAYER_H
#define FIELD_PLAYER_H

#include <iostream>
#include "RealThing.h"

struct FieldPlayer : public RealThing {
    FieldPlayer(RealThingData tD, ThingLists tL);
    ~FieldPlayer();

    void meat(KeyPresses keysDown);
    
    static FieldPlayer *player;
};

#endif
