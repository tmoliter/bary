#ifndef SPRITE_EDITOR_H
#define SPRITE_EDITOR_H
#include "Input.h"
#include "Sprite.h"
#include "Camera.h"
#include "gui/Text.h"
#include "gui/UIRenderer.h"

using namespace std;

enum class SpriteEditState {
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

        Sprite *sprite;
        Thing *focus, *oldFocus;
        SpriteEditState editState, cameraPrevState;
        Text *text;

        bool foundSprite;
        bool snapCamera;

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

        static int checkPath(string input);
};

#endif
