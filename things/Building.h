#ifndef BUILDING_H
#define BUILDING_H

#include "Thing.h"
#include "../components/Walk.h"
#include <iostream>

struct BuildingData : ThingData {
    string name;
};

class Building : public Thing {
    private:
        string name;
    public:
        Building(BuildingData fpD);
        ~Building();

        void meat(KeyPresses keysDown);

        static int write_thing_datum(ifstream &mapData, BuildingData &newTD);
};

#endif
