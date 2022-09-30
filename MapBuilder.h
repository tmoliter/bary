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
#include "components/RayEditor.h"
#include "gui/Text.h"
#include "gui/UIRenderer.h"

using namespace std;

enum EditorState {
    freeMove,
    thingMove,
    commandInput,
    renameThing,
    pathInput,
    spriteSelect,
    spriteEdit,
    rayEdit,
};

class MapBuilder {
    public:
        MapBuilder();

        RealThing *dotThing, *currentThing;

        EditorState state;
        string input;
        Text *commandText, *helpText;

        int selectedSprite;
        SpriteEditor *spriteEditor;
        Sprite *cross;

        RayEditor *rayEditor;

        void changeState(EditorState newState);
        void createOrSelectThing();
        void focusDot();

        void beginTextInput();
        void endTextInput();

        int listenForTextInput(KeyPresses keysDown);
        void meat(KeyPresses keysDown);

        int addSprite();

        static MapBuilder *mapBuilder;
};

#endif
