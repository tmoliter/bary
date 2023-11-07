#ifndef FIELD_PLAYER_H
#define FIELD_PLAYER_H

#include <iostream>
#include "components/Walk.h"

struct FieldPlayerData : RealThingData {
};

class FieldPlayer : public RealThing {
    private:
        string name;
        void init();
    public:
        Walk* walk;
        FieldPlayer(FieldPlayerData fpD);
        FieldPlayer(Point p, string name, string textureName);
        ~FieldPlayer();

        Direction currentDirection;
        Sprite *sprite;

        void getRay(Ray &r);

        void meat(KeyPresses keysDown);
        
        static FieldPlayer *player;
};

#endif
