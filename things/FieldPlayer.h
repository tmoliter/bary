#ifndef FIELD_PLAYER_H
#define FIELD_PLAYER_H

#include <iostream>
#include "Scene.h"

struct FieldPlayerData : RealThingData {
};

struct FieldPlayer : public RealThing {
    void init();

    FieldPlayer(FieldPlayerData fpD);
    FieldPlayer(Point p, string n, string textureName);
    ~FieldPlayer();

    void meat(KeyPresses keysDown);
    
    static FieldPlayer *player;
};

#endif
