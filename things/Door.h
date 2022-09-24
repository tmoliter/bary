#ifndef DOOR_H
#define DOOR_H

#include "Thing.h"
#include "../components/Walk.h"
#include "../components/Sprite.h"
#include "../components/Obstruction.h"
#include <iostream>
#include <vector>

using namespace std;

struct DoorData : ThingData {
    SpriteData spriteDataVector [2];
    vector<CollidableData> obstructionData;
};

class Door : public Thing {
    public:
        Door(DoorData fpD);
        ~Door();

        Sprite* sprites [2];
        vector<Obstruction*> obstructions;

        static Door *find_door(string name);
        static int parse_door_datum(ifstream &mapData, DoorData &newDD);
};

#endif
