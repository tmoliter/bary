#ifndef MAP_BUILDER_H
#define MAP_BUILDER_H
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "globals.h"
#include "Camera.h"
#include "things/Thing.h"
#include "things/RealThing.h"
#include "components/Sprite.h"
#include "gui/UIRenderer.h"

using namespace std;

enum Phase {
    freeMove,
    pathInput,
    stringInput,
    intInput,
};

class MapBuilder {
    public:
        MapBuilder();

        RealThing *dotThing, *currentThing;

        Phase phase;
        int editSpeed, attributeIndex;
        string input;
        Sprite *currentSprite;

        vector<Sprite*> workingSprites;
        vector<int*> intAttrs;
        vector<string*> stringAttrs;
        vector<string> attrNames;

        int addSprite();

        void meat(KeyPresses keysDown);

        static MapBuilder *mapBuilder;

        // stack of editTypes, where editType is enum of what we are editing
        // Might need a second mode enum for whether we are moving around or editing
};

#endif
