#ifndef VISIBLE_THING_H
#define VISIBLE_THING_H

#include "Thing.h"
#include "../components/Walk.h"
#include "../components/Sprite.h"
#include "../components/Obstruction.h"
#include <iostream>
#include <vector>

using namespace std;

struct RealThingData : ThingData {
    vector<SpriteData> spriteDataVector;
    vector<CollidableData> obstructionData;
};

class RealThing : public Thing {
    public:
        RealThing(RealThingData fpD);
        RealThing(Point p, string name);
        RealThing(Point p);
        ~RealThing();

        vector<Sprite*> sprites;
        vector<Obstruction*> obstructions;

        void AddSprite(Sprite* sprite);
        Sprite* AddRawSprite(string path);

        static RealThing *find_building(string name);
        static int parse_building_datum(ifstream &mapData, RealThingData &newTD);
};

#endif
