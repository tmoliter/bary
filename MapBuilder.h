#ifndef MAP_BUILDER_H
#define MAP_BUILDER_H
#include <iostream>
#include <string>
#include "globals.h"
#include "Camera.h"
#include "things/Thing.h"
#include "things/RealThing.h"
#include "components/Sprite.h"
#include "components/SpriteEditor.h"
#include "gui/Text.h"
#include "gui/UIRenderer.h"

using namespace std;

enum EditorState {
    freeMove,
    thingMove,
    pathInput,
    spriteEdit,
};

class MapBuilder {
    public:
        MapBuilder();

        RealThing *dotThing, *currentThing;

        EditorState state;
        string input;
        SpriteEditor *spriteEditor;
        Text *spriteText;

        int addSprite();

        void meat(KeyPresses keysDown);
        void prepareForNextSprite();

        static MapBuilder *mapBuilder;

        // stack of editTypes, where editType is enum of what we are editing
        // Might need a second mode enum for whether we are moving around or editing
};

#endif
