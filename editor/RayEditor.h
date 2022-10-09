#ifndef RAY_EDITOR_H
#define RAY_EDITOR_H
#include "Input.h"
#include "Camera.h"
#include "things/RealThing.h"
#include "events/eventMap.h"
#include "gui/Text.h"
#include "gui/UIRenderer.h"
#include "gui/Line.h"

using namespace std;

enum class RayEditState {
    selectType,
    name,
    layer,
    move,
    stretch,
    select,
    cameraMove
};

class RayEditor {
    public:
        RayEditor(RealThing *p);
        ~RayEditor();

        int layer, selectedI;
        string name;
        Ray* ray;
        Line* line;
        Text *text;

        RealThing *parent;
        Thing *focus, *oldFocus;
        Collidable* selected;

        RayEditState editState, cameraPrevState;
        CollidableType type;

        void saveRay();

        int nextMode();
        int lastMode();

        void handleCameraControls(KeyPresses keysDown);
        void displayText();
        void updateLines();
        void handleNameSubmit();
        void enterSelect();
        void exitSelect();
        void matchSelected();
        void redoRay();

        int routeInput(KeyPresses keysDown);


        void setType (KeyPresses keysDown);
        void editLayer(KeyPresses keysDown);
        void move (KeyPresses keysDown);
        void stretch (KeyPresses keysDown);
        void handleNameInput(KeyPresses keysDown);
        void select(KeyPresses keysDown);
};

#endif
