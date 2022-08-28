#ifndef FIELD_PLAYER_H
#define FIELD_PLAYER_H

#include "Thing.h"
#include "../components/Walk.h"
#include <iostream>

struct FieldPlayerData : ThingData {
    string name;
};

class FieldPlayer : public Thing {
    private:
        Walk* walk;
        string name;
    public:
        FieldPlayer(FieldPlayerData fpD) : Thing(fpD), name(fpD.name) {};

        void init();
        void meat() {};
        void meat(KeyPresses keysDown);
        void destroy(map<int, Thing*>::iterator &itr);

        static int write_thing_datum(ifstream &mapData, FieldPlayerData &newTD);
};
#endif
