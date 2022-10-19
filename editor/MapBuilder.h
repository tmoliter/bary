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
#include "editor/TemplatePicker.h"
#include "gui/Text.h"
#include "gui/UIRenderer.h"

using namespace std;


enum class EditorState {
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
    thingFromTemplate
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
        TemplatePicker *templatePicker;

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
