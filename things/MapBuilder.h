#ifndef MAP_BUILDER_H
#define MAP_BUILDER_H
#include <iostream>
#include <vector>
#include "Thing.h"
#include "globals.h"
#include "../components/Sprite.h"

using namespace std;

class MapBuilder : public Thing {
    public:
        MapBuilder();

        Sprite *sprite;

        void meat(KeyPresses keysDown);

        static MapBuilder *mapBuilder;
};

#endif
