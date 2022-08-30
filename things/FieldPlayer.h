#ifndef FIELD_PLAYER_H
#define FIELD_PLAYER_H

#include "Thing.h"
#include "../Ray.h"
#include "../components/Walk.h"
#include "../components/Obstruction.h"
#include <iostream>

struct FieldPlayerData : ThingData {
    string name;
    SpriteData spriteData;
};

class FieldPlayer : public Thing {
    private:
        Walk* walk;
        string name;
    public:
        FieldPlayer(FieldPlayerData fpD);
        ~FieldPlayer();

        Sprite *sprite;

        void meat(KeyPresses keysDown);

        static int write_player_datum(ifstream &mapData, FieldPlayerData &newTD);
};
#endif
