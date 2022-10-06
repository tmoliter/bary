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
    selectType,
    name,
    layer,
    move,
    stretch,
    cameraMove
};

class RayEditor {
    public:
        RayEditor(RealThing *p);
        ~RayEditor();

        int layer;
        string name;
        Ray* ray;
        Line* line;
        Text *text;

        RealThing *parent;
        Thing *focus, *oldFocus;

        RayEditState editState, cameraPrevState;
        CollidableType type;

        void saveRay();

        int nextMode();
        int lastMode();

        void handleCameraControls(KeyPresses keysDown);
        void displayText();
        void updateLines();
        void handleNameSubmit();

        int routeInput(KeyPresses keysDown);


        void setType (KeyPresses keysDown);
        void editLayer(KeyPresses keysDown);
        void move (KeyPresses keysDown);
        void stretch (KeyPresses keysDown);
        void handleNameInput(KeyPresses keysDown);
};

#endif
