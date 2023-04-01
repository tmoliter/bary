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
    Camera::panTo(rt->name);
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
    thingEditor = new ThingEditor(realThing);
    type = ThingType::thing;
}


void ThingRouter::changeState(ThingRouterState newState) {
    switch (newState) {
        case ThingRouterState::chooseThingType:
            commandLine = new CommandLine({"thing", "door", "free"}, false);
            break;
        case ThingRouterState::editOrCreateSub:
            commandLine = new CommandLine({"edit", "subthings", "free"}, false);
            break;
        case ThingRouterState::edit:
            break;
    }
    state = newState;
}

int ThingRouter::routeInput(KeyPresses keysDown) {
    if (state == ThingRouterState::chooseThingType){
        if (chooseNewThingType(keysDown) > 0) {
            if (type == ThingType::none)
                return 1;
            changeState(ThingRouterState::edit);
            return 0;
        }
        return 0;
    }
    if (state == ThingRouterState::editOrCreateSub){
        if (chooseEditAction(keysDown)) {
            delete commandLine;
            commandLine = nullptr;
            changeState(ThingRouterState::edit);
            return 0;
        }
        return 0;
    }
    if (state == ThingRouterState::edit)
        return editThing(keysDown);
    return 0;
}

int ThingRouter::chooseNewThingType(KeyPresses keysDown) {
    if (!commandLine->handleInput(keysDown))
        return 0;
    string input = commandLine->popInput();
    if (input == "thing") {
        delete commandLine;
        commandLine = nullptr;
        thingEditor = new ThingEditor(newThingPosition);
        type = ThingType::thing;
        return 1;
    }
    if (input == "door") {
        delete commandLine;
        commandLine = nullptr;
        doorEditor = new DoorEditor(newThingPosition);
        type = ThingType::door;
        return 1;
    }
    if (input == "free") {
        type = ThingType::none;
        return 1;
    }
    return 0;
}

int ThingRouter::chooseEditAction(KeyPresses keysDown) {
    if (!commandLine->handleInput(keysDown))
        return 0;
    string input = commandLine->popInput();
    if (input == "edit") {
        determineType();
        changeState(ThingRouterState::edit);
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
