#ifndef BUILDING_H
#define BUILDING_H

#include "Thing.h"
#include "../components/Walk.h"
#include "../components/Obstruction.h"
#include <iostream>
#include <vector>

using namespace std;

struct BuildingData : ThingData {
    vector<SpriteData> spriteDataVector;
    vector<CollidableData> obstructionData;
};

class Building : public Thing {
    public:
        Building(BuildingData fpD);
        ~Building();

        vector<Sprite*> sprites;
        vector<Obstruction*> obstructions;

        static Building *find_building(string name);
        static int write_building_datum(ifstream &mapData, BuildingData &newTD);
};

#endif
