#ifndef FIELD_PLAYER_H
#define FIELD_PLAYER_H

#include "Thing.h"
#include "../Ray.h"
#include "../components/Walk.h"
#include "../components/Sprite.h"
#include "../components/Obstruction.h"
#include "../components/Interactable.h"
#include <iostream>
#include "Camera.h"

struct FieldPlayerData : ThingData {
    SpriteData spriteData;
};

class FieldPlayer : public Thing {
    private:
        string name;
    public:
        Walk* walk;
        FieldPlayer(FieldPlayerData fpD);
        ~FieldPlayer();

        Direction currentDirection;
        Sprite *sprite;

        void getRay(Ray &r);

        void meat(KeyPresses keysDown);

        static int parse_player_datum(ifstream &mapData, FieldPlayerData &newTD);
        
        static FieldPlayer *player;
};

#endif
