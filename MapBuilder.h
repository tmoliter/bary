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
    commandInput,
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
        Text *commandText;

        void changeState(EditorState newState);

        void beginTextInput();
        void endTextInput();

        int listenForTextInput(KeyPresses keysDown);
        void meat(KeyPresses keysDown);

        int addSprite();

        static MapBuilder *mapBuilder;
};

#endif
