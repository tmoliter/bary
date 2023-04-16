#ifndef FIELD_PLAYER_H
#define FIELD_PLAYER_H

#include <iostream>
#include "../components/Walk.h"
#include "things/RealThing.h"
#include "Camera.h"

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

        static int parse_player_datum(ifstream &mapData, FieldPlayerData &newTD);
        
        static FieldPlayer *player;
};

#endif
