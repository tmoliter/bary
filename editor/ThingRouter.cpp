#include "editor/ThingRouter.h"

ThingRouter::ThingRouter(Point p) : state(ThingRouterState::chooseThingType), newThingPosition(p), type(ThingType::thing) {
    thingEditor = nullptr;
    realThing = nullptr;
    cross = nullptr;
    changeState(ThingRouterState::chooseThingType);
}

ThingRouter::ThingRouter(RealThing *rt) : realThing(rt), state(ThingRouterState::editOrCreateSub), newThingPosition(rt->position) {
    thingEditor = nullptr;
    cross = nullptr;
    rt->highlightThing();
    changeState(ThingRouterState::editOrCreateSub);
    FocusTracker::ftracker->setFocus(rt);
}

ThingRouter::~ThingRouter(){
    destroyCross();
    delete thingEditor;
}

void ThingRouter::determineType() {
    thingEditor = new ThingEditor(realThing);
    type = ThingType::thing;
}

void ThingRouter::createCross() {
    if (cross == nullptr && realThing != nullptr) {
        SpriteData sd;
        sd.textureName = "editorCross";
        sd.layer = 100;
        cross = new Sprite(realThing->position, sd);
        cross->centerOffset();
    }
}

void ThingRouter::destroyCross() {
    if (cross != nullptr) {
        delete cross;
        cross = nullptr;
    }
}

void ThingRouter::changeState(ThingRouterState newState) {
    switch (newState) {
        case ThingRouterState::chooseThingType:
            destroyCross();
            CommandLine::refresh({"thing", "free"}, CLIMode::typeCommand);
            break;
        case ThingRouterState::editOrCreateSub:
            destroyCross();
            CommandLine::refresh({"edit", "subthings", "free", "print"}, CLIMode::typeCommand);
            break;
        case ThingRouterState::edit:
            createCross();
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
        realThing = thingEditor->thing;
        type = ThingType::thing;
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
    if (input == "print") {
        CommandLine::breakdown();
        lua_State* L = realThing->L;
        if (!CheckLua(L, luaL_dofile(L, "scripts/save.lua"))) {
            return 1;
        }
        lua_getglobal(L, "printThing");
        if (!lua_isfunction(L, -1)) {
            cout << "FAILED TO PRINT! printThing is not a function!" << endl;
            return 1 ;
        }
        realThing->PushThingDataOnStack();
        lua_pushstring(L, static_cast<Scene*>(realThing->parentScene)->sceneName.c_str());
        if (!luaUtils::CheckLua(L, lua_pcall(L, 2, 0, 0))) {
            cout << "FAILED TO PRINT!" << endl;
        }
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
    return 0;
}
