#ifndef DOOR_EDITOR_H
#define DOOR_EDITOR_H
#include "editor/SpriteEditor.h"
#include "editor/RayEditor.h"

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
    DoorEditor(Door *door);
    ~DoorEditor();

    string input;
    SpriteEditor *spriteEditor;
    RayEditor *rayEditor;
    DoorEditState state;
    Door *door;
    Text *text;

    void changeState(DoorEditState newState);
    void updateText();

    int nextMode();
    int lastMode();

    int routeInput(KeyPresses keysDown);

    void handleTextInput(KeyPresses keysDown);
};

#endif
