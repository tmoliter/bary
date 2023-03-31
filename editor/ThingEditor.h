#ifndef MAP_BUILDER_H
#define MAP_BUILDER_H
#include <iostream>
#include <string>
#include "globals.h"
#include "Camera.h"
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
        ThingEditor();
        ~ThingEditor();

        RealThing *thing;
        ThingEditState state;
        string input;
        vector<string> spritePathHistory;
        Text *helpText;

        int selectedSprite;

        CommandLine *commandLine;
        SpriteEditor *spriteEditor;
        RayEditor *rayEditor;
        EventEditor *eventEditor;

        void changeState(ThingEditState newState);

        int meat(KeyPresses keysDown);

        int addSprite();
};


#endif