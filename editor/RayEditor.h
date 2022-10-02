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

enum class RayEditState {
    move,
    stretch,
    layer,
    cameraMove
};

enum class RayType {
    obstruction,
    interactable
};

class RayEditor {
    public:
        RayEditor(RealThing *p);
        ~RayEditor();

        int layer;
        Ray* ray;
        Line* line;

        RealThing *parent;
        Thing *focus, *oldFocus;
        RayEditState editState, cameraPrevState;
        Text *text;

        void saveRay();

        int nextMode();
        int lastMode();

        void handleCameraControls(KeyPresses keysDown);
        void displayText();

        int routeInput(KeyPresses keysDown);

        void move (KeyPresses keysDown);
        void stretch (KeyPresses keysDown);
        void editLayer(KeyPresses keysDown);
};

#endif
