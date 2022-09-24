#ifndef SPRITE_EDITOR_H
#define SPRITE_EDITOR_H
#include "Input.h"
#include "Sprite.h"
#include "Camera.h"
#include "gui/Text.h"
#include "gui/UIRenderer.h"

using namespace std;

enum EditState {
    move,
    grow,
    shrink,
    layer,
    renderOffset,
    cameraMove
};

class SpriteEditor {
    public:
        SpriteEditor(Sprite *s);
        ~SpriteEditor();

        Sprite *sprite, *cross;
        Thing *focus, *oldFocus;
        EditState editState, cameraPrevState;
        Text *text;

        int editSpeed;

        int nextMode();
        int lastMode();

        void handleCameraControls(KeyPresses keysDown);
        void changeEditSpeed(KeyPresses keysDown);
        void frontAndCenter(KeyPresses keysDown);
        void displayText();

        int routeInput(KeyPresses keysDown);

        void move (KeyPresses keysDown);
        void shrink (KeyPresses keysDown);
        void grow (KeyPresses keysDown);
        void editRenderOffset(KeyPresses keysDown);
        void editLayer(KeyPresses keysDown);
};

#endif
