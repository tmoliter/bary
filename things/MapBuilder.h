#ifndef MAP_BUILDER_H
#define MAP_BUILDER_H
#include <iostream>
#include <fstream>
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

        int editSpeed;
        string input;
        Sprite *sprite;

        void setSprite();

        void meat(KeyPresses keysDown);

        static MapBuilder *mapBuilder;

        // stack of editTypes, where editType is enum of what we are editing
        // Might need a second mode enum for whether we are moving around or editing
};

#endif
