#ifndef MAP_BUILDER_H
#define MAP_BUILDER_H
#include <iostream>
#include <string>
#include "editor/ThingRouter.h"

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

        RayEditor *rayEditor;
        EventEditor *eventEditor;
        ThingRouter *thingRouter;

        void changeState(EditorState newState);
        void focusDot();
        void updateLines();

        void meat(KeyPresses keysDown);

        static MapBuilder *mapBuilder;
};

#endif
