#ifndef FIELD_PLAYER_H
#define FIELD_PLAYER_H

#include <iostream>
#include "RealThing.h"

struct FieldPlayer : public RealThing {
    FieldPlayer(RealThingData tD, ThingLists tL);
    ~FieldPlayer();

    void meat(KeyPresses keysDown);

    int castRayForInteractables ();
    int castRayForTriggers ();
    
    static FieldPlayer *player;
};

#endif
