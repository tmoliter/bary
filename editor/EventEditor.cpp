#include "EventEditor.h"

EventEditor::EventEditor(RealThing *p) : 
parent(p), 
input(""),
eventName(""),
collidable(make_pair("",nullptr)),
collidableType(CollidableType::interactable), 
eventType(EventType::simpleMessage) {
    parent->showInteractableLines();
    text = new Text(Point(LETTER_WIDTH * 2, LETTER_HEIGHT * 4), "");
    UIRenderer::addText(text);
    changeState(EventEditState::selectCollidableType);
};

EventEditor::~EventEditor() {
    UIRenderer::removeText(text);
};


int EventEditor::changeState(EventEditState nextState) {
    switch(nextState) {
        int totalInteractables;
        int totalTriggers;
        case EventEditState::selectCollidableType:
            totalInteractables = parent->interactables.size();
            totalTriggers = parent->triggers.size();
            collidable = make_pair("",nullptr);
            if (totalInteractables < 1 && totalTriggers < 1)
                return 1;
            if (totalInteractables < 1) {
                collidableType = CollidableType::trigger;
                changeState(EventEditState::chooseCollidable);
                return 0;
            }
            if (totalTriggers < 1) {
                collidableType = CollidableType::interactable;
                changeState(EventEditState::chooseCollidable);
                return 0;
            }
            gameState = GameState::FieldFree;
            editState = nextState;
            updateDisplay();
            return 0;
        case EventEditState::chooseCollidable:
            totalInteractables = parent->interactables.size();
            totalTriggers = parent->triggers.size();
            collidable = make_pair("",nullptr);
            if (collidableType == CollidableType::interactable && totalInteractables == 1) {
                collidable = *parent->interactables.begin();
                changeState(EventEditState::selectEventType);
                return 0;
            }
            if (collidableType == CollidableType::trigger && totalTriggers == 1) {
                collidable = *parent->triggers.begin();
                changeState(EventEditState::selectEventType);
                return 0;
            }
            if (collidableType == CollidableType::interactable)
                availableCollidables = parent->findAndShowInteractableLines(input);
            else
                availableCollidables = parent->findAndShowTriggerLines(input);
            gameState = GameState::TextInput;
            editState = nextState;
            updateDisplay();
            return 0;
        case EventEditState::selectEventType:
            if (eventMap::eventNameToGenerator.size() < 1) {
                eventType = EventType::simpleMessage;
                changeState(EventEditState::enterMessage);
                return 0;
            }
            gameState = GameState::FieldFree;
            editState = nextState;
            updateDisplay();
            return 0;
        case EventEditState::choosePredefined:
            gameState = GameState::TextInput;
            editState = nextState;
            updateDisplay();
            return 0;
        case EventEditState::predefinedSuccess:
            gameState = GameState::FieldFree;
            editState = nextState;
            updateDisplay();
            return 0;
        default:
            return 0;
    }
    return 0;
}


void EventEditor::updateDisplay() {
    string displayText;
    string prefix;
    switch (editState) {
        case EventEditState::selectCollidableType:
            text->clearText();
            displayText = "Select collidable type:` " + 
                string(collidableType == CollidableType::interactable ? "interactable" : "trigger");
            break;
        case EventEditState::chooseCollidable:
            text->clearText();
            displayText =  "Enter collidable name` " + input + "`  List:";
            for (auto name : availableCollidables) {
                displayText = displayText + "`  " + eventMap::namePlusEvent(parent->name, name, collidableType);
            }
            break;
        case EventEditState::selectEventType:
            text->clearText();
            displayText =  "Select event type for collidable " + collidable.first + 
                string(eventType == EventType::predefined ? + ":` predefined" : ":` simple message" );
            break;
        case EventEditState::choosePredefined:
            text->clearText();
            displayText =  "Enter event name from list for collidable '" + 
                collidable.first + "'` "
                + input + "`  List:";
            for (auto const& [name, fnptr] : eventMap::eventNameToGenerator)
                if (input.size() < 1 || name.rfind(input, 0) == 0)
                    displayText = displayText + "`  " + name;
            break;
        case EventEditState::predefinedSuccess:
            text->clearText();
            displayText =  "Successfully added event '" + eventName + "' to collidable '" + collidable.first + "'!";
            break;
        default:
            break;
    }
    text->setText(displayText);
}


int EventEditor::routeInput(KeyPresses keysDown) {
    switch (editState) {
    case EventEditState::selectCollidableType:
        selectCollidableType(keysDown);
        break;
    case EventEditState::chooseCollidable:
        chooseCollidable(keysDown);
        break;
    case EventEditState::selectEventType:
        selectEventType(keysDown);
        break;
    case EventEditState::choosePredefined:
        choosePredefined(keysDown);
        break;
    case EventEditState::predefinedSuccess:
        if (keysDown.ok)
            return 1;
    default:
        break;
    }
    return 0;
}

void EventEditor::selectCollidableType (KeyPresses keysDown) {
    if (keysDown.ok) {
        changeState(EventEditState::chooseCollidable);
    }
    if( keysDown.up || keysDown.debug_up) {
        collidableType = CollidableType::interactable;
        parent->showInteractableLines();
        updateDisplay();
    }
    if (keysDown.down || keysDown.debug_down) {
        collidableType = CollidableType::trigger;
        parent->showTriggerLines();
        updateDisplay();
    }
}

void EventEditor::chooseCollidable (KeyPresses keysDown) {
    if (keysDown.textInput){
        input.push_back(keysDown.textInput);
        if (collidableType == CollidableType::interactable)
            availableCollidables = parent->findAndShowInteractableLines(input);
        else
            availableCollidables = parent->findAndShowTriggerLines(input);
        updateDisplay();
    }
    if (keysDown.del && input.length() > 0){
        input.pop_back();
        if (collidableType == CollidableType::interactable)
            availableCollidables = parent->findAndShowInteractableLines(input);
        else
            availableCollidables = parent->findAndShowTriggerLines(input);
        updateDisplay();
    }
    if (keysDown.start && availableCollidables.size() == 1) {
        if (collidableType == CollidableType::interactable)
            collidable = make_pair(availableCollidables.back(), parent->interactables[availableCollidables.back()]);
        else
            collidable = make_pair(availableCollidables.back(), parent->triggers[availableCollidables.back()]);
        changeState(EventEditState::selectEventType);
    }
}


void EventEditor::selectEventType (KeyPresses keysDown) {
    if (keysDown.ok) {
        switch (eventType) {
            case EventType::simpleMessage:
                changeState(EventEditState::enterMessage);
                return;
            case EventType::predefined:
                changeState(EventEditState::choosePredefined);
                return;
        }
    }
    if( keysDown.up || keysDown.debug_up) {
        switch (eventType) {
            case EventType::simpleMessage:
            case EventType::predefined:
                eventType = EventType::simpleMessage;
                updateDisplay();
                break;
        }
    }
    if (keysDown.down || keysDown.debug_down) {
        switch (eventType) { 
            case EventType::simpleMessage:
            case EventType::predefined:
                eventType = EventType::predefined;
                updateDisplay();
                break;
        }
    }
}

void EventEditor::choosePredefined (KeyPresses keysDown) {
    if (keysDown.textInput){
        input.push_back(keysDown.textInput);
        updateDisplay();
    }
    if (keysDown.del && input.length() > 0){
        input.pop_back();
        updateDisplay();
    }
    if (keysDown.start) {
        vector<string> availableEvents;
        for (auto const& [name, fnptr] : eventMap::eventNameToGenerator)
            if (input.size() < 1 || name.rfind(input, 0) == 0)
                availableEvents.push_back(name);
        if(availableEvents.size() == 1) {
            eventName = availableEvents.back();
            eventMap::attachEvent(eventName, parent->name, collidable.first, collidableType);
            changeState(EventEditState::predefinedSuccess);
        }
    }
}