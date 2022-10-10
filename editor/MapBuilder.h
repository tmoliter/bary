#ifndef MAP_BUILDER_H
#define MAP_BUILDER_H
#include <iostream>
#include <string>
#include "globals.h"
#include "Camera.h"
#include "things/FieldPlayer.h"
#include "editor/SpriteEditor.h"
#include "editor/RayEditor.h"
#include "editor/EventEditor.h"
#include "gui/Text.h"
#include "gui/UIRenderer.h"

using namespace std;


// TODO: EVENT BUILDER (new class) (enter thing name [do we dedupe and expand suffixes?] -> choose trig/inter -> choose from list -> choose named event or simple message)
// TODO: "play" COMMAND (makes a FieldPlayer and focuses them, but still allows pressing START -> START deletes FieldPlayer and returns to freemove)


enum EditorState {
    freeMove,
    play,
    thingMove,
    commandInput,
    renameThing,
    pathInput,
    spriteSelect,
    spriteEdit,
    rayEdit,
    eventEdit,
};

class MapBuilder {
    public:
        MapBuilder();

        RealThing *dotThing, *currentThing;

        EditorState state;
        string input, lastPath;
        Text *commandText, *helpText, *commandList;

        int selectedSprite;
        SpriteEditor *spriteEditor;
        Sprite *cross;

        RayEditor *rayEditor;
        EventEditor *eventEditor;

        void changeState(EditorState newState);
        void createOrSelectThing();
        void focusDot();
        void updateLines();

        void beginTextInput();
        void endTextInput();

        int listenForTextInput(KeyPresses keysDown);
        void meat(KeyPresses keysDown);

        int addSprite();

        static MapBuilder *mapBuilder;
};

#endif
