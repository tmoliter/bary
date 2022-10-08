#ifndef EVENT_EDITOR_H
#define EVENT_EDITOR_H
#include "Input.h"
#include "Camera.h"
#include "things/RealThing.h"
#include "gui/Text.h"
#include "gui/UIRenderer.h"
#include "gui/Line.h"

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
};

class EventEditor {
    public:
        EventEditor(RealThing *p, Text *&t) : parent(p), input(""), editState(EventEditState::chooseCollidable), text(t) {};
        ~EventEditor();

        string input;
        Text *&text;

        RealThing* parent;
        EventCollidable* collidable;

        EventEditState editState;
        EventType type;

        int changeState(EventEditState nextState);

        void updateDisplay();
        void updateLines();

        int routeInput(KeyPresses keysDown);

        void selectType (KeyPresses keysDown);
        void enterInput (KeyPresses keysDown);
};

#endif
