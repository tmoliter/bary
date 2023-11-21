#ifndef FIELD_PLAYER_H
#define FIELD_PLAYER_H

#include <iostream>
#include "Scene.h"

struct FieldPlayer : public RealThing {
    void init();

    FieldPlayer(RealThingData tD, ThingLists tL, string textureName = "");
    ~FieldPlayer();

    void meat(KeyPresses keysDown);
    
    static FieldPlayer *player;
};

#endif
