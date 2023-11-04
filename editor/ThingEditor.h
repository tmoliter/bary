#ifndef THING_EDITOR_H
#define THING_EDITOR_H
#include <iostream>
#include <string>
#include "globals.h"
#include "editor/SpriteEditor.h"
#include "editor/RayEditor.h"
#include "editor/EventEditor.h"
#include "editor/CommandLine.h"

enum class ThingEditState {
    move,
    commandInput,
    rename,
    pathInput,
    spriteSelect,
    spriteEdit,
    rayEdit,
    eventEdit,
};


class ThingEditor {
    public:
        ThingEditor(Point p);
        ThingEditor(RealThing *rt);
        ~ThingEditor();

        void init();

        RealThing *thing;
        ThingEditState state;
        Text *helpText;

        int selectedSprite;

        SpriteEditor *spriteEditor;
        RayEditor *rayEditor;
        EventEditor *eventEditor;

        void changeState(ThingEditState newState);

        int meat(KeyPresses keysDown);

        int addSprite();
};


#endif