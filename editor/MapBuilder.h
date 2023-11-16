#ifndef MAP_BUILDER_H
#define MAP_BUILDER_H
#include <iostream>
#include <string>
#include "editor/ThingRouter.h"

using namespace std;
using namespace luaUtils;


enum class EditorState {
    freeMove,
    play,
    commandInput,
    thingEdit,
};

class MapBuilder {
    public:
        MapBuilder();

        Scene* scene;

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
        void save();

        void meat(KeyPresses keysDown);

        static MapBuilder *mapBuilder;
};

#endif
