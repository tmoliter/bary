#include "EventEditor.h"

// https://stackoverflow.com/questions/1878001/how-do-i-check-if-a-c-stdstring-starts-with-a-certain-string-and-convert-a
// Need a showEventSubsetLines() function that takes a vector of names and only shows triggers/interactables in that list
EventEditor::~EventEditor() {
    UIRenderer::removeText(text);
};


int EventEditor::changeState(EventEditState nextState) {
    switch(nextState) {
        case EventEditState::enterMessage:
        default:
            return 1;
            break;
    }
    return 0;
}


void EventEditor::updateDisplay() {
    string displayText;
    string prefix = "Type: " + string(type == EventType::simpleMessage ? "Simple Message" : "Predefined") + "` ";
    switch (editState) {
        case EventEditState::selectCollidableType:
            displayText = "select type";
            break;
        case EventEditState::chooseCollidable:
            displayText = "select type";
            break;
        default:
            break;
    }
    displayText = displayText + "` " + input;
    text->setText(displayText);
}


int EventEditor::routeInput(KeyPresses keysDown) {
    switch (editState) {
    case EventEditState::selectCollidableType:
        selectType(keysDown);
        break;
    default:
        break;
    }
    return 0;
}

void EventEditor::selectType (KeyPresses keysDown) {
    if (keysDown.ok) {
        switch (type) {
            case EventType::simpleMessage:
                changeState(EventEditState::enterMessage);
                return;
            case EventType::predefined:
                return;
        }
    }
    if( keysDown.up || keysDown.debug_up) {
        switch (type) {
            case EventType::simpleMessage:
            case EventType::predefined:
                type = EventType::simpleMessage;
                break;
        }
    }
    if (keysDown.down || keysDown.debug_down) {
        switch (type) {
            case EventType::simpleMessage:
            case EventType::predefined:
                type = EventType::predefined;
                break;
        }
    }
}

void EventEditor::enterInput (KeyPresses keysDown) {
    if (keysDown.textInput) {
        input.push_back(keysDown.textInput);
        updateDisplay();
    }
    if (keysDown.del && input.length() > 0) {
        input.pop_back();
        updateDisplay();
    }
    if (keysDown.start) {
        switch (editState) {
            case EventEditState::chooseCollidable:
                // changestate etc
                break;
            default:
                break;
            return;
        }
    }
}
