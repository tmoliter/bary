#ifndef EVENT_EDITOR_H
#define EVENT_EDITOR_H
#include "util.h"
#include "things/RealThing.h"
#include "events/eventMap.h"
#include "events/SimpleMessage.h"

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
    enterSoundName,
    editBox,
    messageSuccess,
    choosePredefined,
    predefinedSuccess,
};

enum class BoxEditState {
    resize,
    padding,
    move,
};

class EventEditor {
    public:
        EventEditor(RealThing *p);
        ~EventEditor();

        string input, eventName, soundName;
        Text *text;

        RealThing* parent;
        pair<string, EventCollidable*> collidable;
        Phrase* previewPhrase;
        
        MenuDisplay* menu;

        vector<string> availableCollidables;

        EventEditState editState;
        BoxEditState boxState;
        EventType eventType;
        CollidableType collidableType;

        int changeState(EventEditState nextState);

        void updateDisplay();

        int routeInput(KeyPresses keysDown);

        void selectCollidableType (KeyPresses keysDown);
        void selectEventType (KeyPresses keysDown);
        void chooseCollidable (KeyPresses keysDown);
        void choosePredefined (KeyPresses keysDown);
        void enterMessage (KeyPresses keysDown);
        void enterSoundName (KeyPresses keysDown);
        void editBox (KeyPresses keysDown);
};

#endif
