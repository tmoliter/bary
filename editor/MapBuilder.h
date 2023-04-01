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
#include "editor/ThingRouter.h"
#include "editor/ThingEditor.h"
#include "gui/Text.h"
#include "gui/UIRenderer.h"

using namespace std;


enum class EditorState {
    freeMove,
    play,
    commandInput,
    thingEdit,
};

class MapBuilder {
    public:
        MapBuilder();

        RealThing *dotThing, *currentThing;

        EditorState state;
        Text *helpText;

        int selectedSprite;
        SpriteEditor *spriteEditor;
        Sprite *cross;

        CommandLine *commandLine;

        RayEditor *rayEditor;
        EventEditor *eventEditor;
        ThingRouter *thingRouter;

        void changeState(EditorState newState);
        void createOrSelectThing();
        void focusDot();
        void updateLines();

        void meat(KeyPresses keysDown);

        int addSprite();

        static MapBuilder *mapBuilder;
};

#endif
