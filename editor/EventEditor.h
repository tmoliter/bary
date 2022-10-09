#ifndef EVENT_EDITOR_H
#define EVENT_EDITOR_H
#include "Input.h"
#include "Camera.h"
#include "things/RealThing.h"
#include "gui/Text.h"
#include "gui/UIRenderer.h"
#include "gui/Line.h"
#include "events/eventMap.h"

using namespace std;

enum class EventType {
    simpleMessage,
    predefined
};


enum class EventEditState {
    selectCollidableType,
    chooseCollidable,
    selectEventType,
    enterMessage,
    positionBox,
    messageSuccess,
    choosePredefined,
    predefinedSuccess,
};

class EventEditor {
    public:
        EventEditor(RealThing *p);
        ~EventEditor();

        string input, eventName;
        Text *text;

        RealThing* parent;
        pair<string, EventCollidable*> collidable;

        vector<string> availableCollidables;

        EventEditState editState;
        EventType eventType;
        CollidableType collidableType;

        int changeState(EventEditState nextState);

        void updateDisplay();

        int routeInput(KeyPresses keysDown);

        void selectCollidableType (KeyPresses keysDown);
        void selectEventType (KeyPresses keysDown);
        void chooseCollidable (KeyPresses keysDown);
        void choosePredefined (KeyPresses keysDown);
};

#endif
