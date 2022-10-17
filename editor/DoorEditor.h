#ifndef DOOR_EDITOR_H
#define DOOR_EDITOR_H
#include "editor/SpriteEditor.h"
#include "editor/RayEditor.h"
#include "things/Door.h"

using namespace std;

enum class DoorEditState {
    closedSprite,
    openSprite,
    move,
    drawLines,
    // lockedCondition,
    // lockedMessage,
};

struct DoorEditor {
    DoorEditor(Point p);
    ~DoorEditor();

    string input;
    SpriteEditor *spriteEditor;
    RayEditor *rayEditor;
    DoorEditState state;
    Door *door;
    Text *text;

    void updateText();

    int nextMode();
    int lastMode();

    int routeInput(KeyPresses keysDown);

    void handleTextInput(KeyPresses keysDown);
};

#endif
