#ifndef VISIBLE_THING_H
#define VISIBLE_THING_H

#include "Thing.h"
#include "../components/Walk.h"
#include "../components/Sprite.h"
#include "../components/Obstruction.h"
#include "../components/Interactable.h"
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

        void _save_name_and_save_in_map(string n);

        vector<Sprite*> sprites;
        vector<Obstruction*> obstructions;
        vector<Interactable*> interactables;

        void calculateHeight();

        Sprite* AddSprite(Sprite* sprite);
        Sprite* AddRawSprite(string path);

        Interactable* addInteractable(vector<Ray*> rays, int layer, Event* event = nullptr);
        Obstruction* addObstruction(vector<Ray*> rays, int layer);
        Interactable* addInteractable();
        Obstruction* addObstruction();

        void RemoveSprite(Sprite* sprite);

        void highlightSprite(Sprite* sprite);
        void removeHighlight();

        static RealThing *find_building(string name);
        static int parse_building_datum(ifstream &mapData, RealThingData &newTD);
};

#endif
