#ifndef MAP_BUILDER_H
#define MAP_BUILDER_H
#include <iostream>
#include <vector>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Thing.h"
#include "globals.h"
#include "../components/Sprite.h"
#include "../gui/UIRenderer.h"

using namespace std;

class MapBuilder : public Thing {
    public:
        MapBuilder();

        string input;
        Sprite *sprite;

        void meat(KeyPresses keysDown);

        static MapBuilder *mapBuilder;
};

#endif
