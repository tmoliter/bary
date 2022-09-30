#ifndef RAY_EDITOR_H
#define RAY_EDITOR_H
#include "Input.h"
#include "Camera.h"
#include "gui/Text.h"
#include "gui/UIRenderer.h"
#include "gui/Line.h"
#include "Ray.h"
#include "things/RealThing.h"

using namespace std;

enum RayEditState {
    rMove,
    stretch,
    rCameraMove
};

class RayEditor {
    public:
        RayEditor(RealThing *p);
        ~RayEditor();

        Ray* ray;
        Line* line;

        RealThing *parent;
        Thing *focus, *oldFocus;
        RayEditState editState, cameraPrevState;
        Text *text;

        int nextMode();
        int lastMode();

        void handleCameraControls(KeyPresses keysDown);
        void displayText();

        int routeInput(KeyPresses keysDown);

        void move (KeyPresses keysDown);
        void stretch (KeyPresses keysDown);
};

#endif
