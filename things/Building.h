#ifndef BUILDING_H
#define BUILDING_H

#include "Thing.h"
#include "../components/Walk.h"
#include "../components/Obstruction.h"
#include <iostream>
#include <vector>

using namespace std;

struct BuildingData : ThingData {
    string name;
    vector<SpriteData> spriteDataVector;
    CollidableData obstructionData;
};

class Building : public Thing {
    private:
        string name;
    public:
        Building(BuildingData fpD);
        ~Building();

        vector<Sprite*> sprites;
        Obstruction obstruction;

        static int write_building_datum(ifstream &mapData, BuildingData &newTD);
};

#endif
