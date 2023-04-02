#include "editor/ThingRouter.h"

ThingRouter::ThingRouter(Point p) : state(ThingRouterState::chooseThingType), newThingPosition(p), type(ThingType::thing) {
    doorEditor = nullptr;
    thingEditor = nullptr;
    realThing = nullptr;
    changeState(ThingRouterState::chooseThingType);
}

ThingRouter::ThingRouter(RealThing *rt) : realThing(rt), state(ThingRouterState::editOrCreateSub), newThingPosition(rt->position) {
    doorEditor = nullptr;
    thingEditor = nullptr;
    Sprite::highlightThing(rt->name);
    changeState(ThingRouterState::editOrCreateSub);
    Camera::panTo(rt->name, true);
}

ThingRouter::~ThingRouter(){
    delete thingEditor;
    delete doorEditor;
}

void ThingRouter::determineType() {
    Door *door = dynamic_cast<Door*>(realThing);
    if (door) {
        doorEditor = new DoorEditor(door);
        type = ThingType::door;
        return;
    }
    // Repeat "door" pattern for other template thing types, fall back to ThingEditor
    thingEditor = new ThingEditor(realThing);
    type = ThingType::thing;
}


void ThingRouter::changeState(ThingRouterState newState) {
    switch (newState) {
        case ThingRouterState::chooseThingType:
            CommandLine::refresh({"thing", "door", "free"}, false);
            break;
        case ThingRouterState::editOrCreateSub:
            CommandLine::refresh({"edit", "subthings", "free"}, false);
            break;
        case ThingRouterState::edit:
            break;
    }
    state = newState;
}

int ThingRouter::routeInput(KeyPresses keysDown) {
    if (state == ThingRouterState::chooseThingType)
        return chooseNewThingType(keysDown);
    if (state == ThingRouterState::editOrCreateSub)
        return chooseEditAction(keysDown);
    if (state == ThingRouterState::edit)
        return editThing(keysDown);
    return 0;
}

int ThingRouter::chooseNewThingType(KeyPresses keysDown) {
    if (!CommandLine::handleInput(keysDown))
        return 0;
    string input = CommandLine::popInput();
    if (input == "thing") {
        CommandLine::breakdown();
        thingEditor = new ThingEditor(newThingPosition);
        type = ThingType::thing;
        changeState(ThingRouterState::edit);
        return 0;
    }
    if (input == "door") {
        CommandLine::breakdown();
        doorEditor = new DoorEditor(newThingPosition);
        type = ThingType::door;
        changeState(ThingRouterState::edit);
        return 0;
    }
    if (input == "free") {
        CommandLine::breakdown();
        return 1;
    }
    return 0;
}

int ThingRouter::chooseEditAction(KeyPresses keysDown) {
    if (!CommandLine::handleInput(keysDown))
        return 0;
    string input = CommandLine::popInput();
    if (input == "edit") {
        determineType();
        changeState(ThingRouterState::edit);
        return 0;
    }
    if (input == "subthing") {
        // Do stuff
        return 0;
    }
    if (input == "free") {
        CommandLine::breakdown();
        return 1;
    }
    return 0;
}


int ThingRouter::editThing(KeyPresses keysDown) {
    if (type == ThingType::thing) {
        if (thingEditor->meat(keysDown)) {
            delete thingEditor;
            thingEditor = nullptr;
            return 1;
        }
    }
    if (type == ThingType::door) {
        if (doorEditor->routeInput(keysDown)) {
            delete doorEditor;
            doorEditor = nullptr;
            return 1;
        }
    }
    return 0;
}
