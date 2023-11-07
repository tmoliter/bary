#include "MapBuilder.h"

MapBuilder *MapBuilder::mapBuilder = nullptr;

MapBuilder::MapBuilder() : selectedSprite(-1) {
    mapBuilder = this;
    cross = nullptr;
    spriteEditor = nullptr;
    rayEditor = nullptr;
    eventEditor = nullptr;
    thingRouter = nullptr;

    CommandLine::init();

    // Maybe we can pass this down into Editors to share? Maybe that's dumb.
    helpText = new Text(Point(16, 16), "");
    UIRenderer::addText(helpText);
    
    currentThing = dotThing = new RealThing(Point(0,0), "EditorDot");
    dotThing->AddRawSprite("singlepixel");
    changeState(EditorState::freeMove);
}

void MapBuilder::changeState(EditorState newState) {
    string prefix = currentThing != dotThing ? currentThing->name + ": " : "";
    switch (newState) {
        case EditorState::freeMove:
            helpText->setText("Free Move");
            focusDot();
            Sprite::removeHighlight();
            state = EditorState::freeMove;
            break;
        case EditorState::play:
            helpText->setText("Play");
            currentThing = new FieldPlayer(dotThing->position, "test player", "zinnia");
            FocusTracker::panTo(currentThing->name, true);
            state = EditorState::play;
            break;
        case EditorState::commandInput:
            helpText->setText("");
            CommandLine::refresh({"play", "new thing", "free", "save"}, CLIMode::typeCommand);
            state = EditorState::commandInput;
            break;
        case EditorState::thingEdit:
            helpText->clearText();
            state = EditorState::thingEdit;
            break;
    }
    updateLines();
}

void MapBuilder::focusDot() {
    currentThing = dotThing;
    FocusTracker::panTo(currentThing->name, true);
    if (cross == nullptr)
        return;
    delete cross;
    cross = nullptr;
}

void MapBuilder::updateLines() {
    if(currentThing == dotThing)
        RealThing::showAllLines();
    else {
        RealThing::hideAllLines();
        currentThing->showLines();
    }
}

void MapBuilder::meat(KeyPresses keysDown) {
    DirectionMap dM;
    if (state == EditorState::freeMove) {
        currentThing->manuallyControl(keysDown);
        if(keysDown.start) {
            changeState(EditorState::commandInput);
            return;
        }
    }

    if(state == EditorState::play) {
        if (keysDown.start) {
            delete currentThing;
            currentThing = dotThing;
            changeState(EditorState::freeMove);
            return;
        }
    }

    if (state == EditorState::freeMove) {
        if (keysDown.ok) {
            vector<RealThing*> collisions = RealThing::findThingsByPoint(dotThing->position);
            for (auto t : collisions) {
                if (t != dotThing) {
                    RealThing* match = dynamic_cast<RealThing*>(t);
                    // TODO should actually make a vector out of ALL matches and pass them in so we can choose which one we want to deal with
                    if (match) {
                        thingRouter = new ThingRouter(match);
                        changeState(EditorState::thingEdit);
                        return;
                    }
                }
            }
        }
    }


    if (state == EditorState::commandInput) {
        if (!CommandLine::handleInput(keysDown))
            return;
        string input = CommandLine::popInput();
        if (input == "")
            return;
        CommandLine::breakdown();
        if (input == "play") {
            changeState(EditorState::play);
            return;
        }
        if (input == "free") {
            changeState(EditorState::freeMove);
            return;
        }
        if (input == "new thing") {
            thingRouter = new ThingRouter(currentThing->position);
            changeState(EditorState::thingEdit);
            return;
        }
        if (input == "save") {
            save();
            changeState(EditorState::freeMove);
            return;
        }
    }

    if(state == EditorState::thingEdit) {
        if(thingRouter->routeInput(keysDown)) {
            delete thingRouter;
            thingRouter = nullptr;
            changeState(EditorState::freeMove);
        }
    }
}

void MapBuilder::save() {
    vector<RealThingData> allThingData = RealThing::getAllThingData();
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    if (CheckLua(L, luaL_dofile(L, "save.lua"))) {
        lua_getglobal(L, "dump");
        if (lua_isfunction(L, -1)) {
            lua_newtable(L);
            for (int i = 0; i < allThingData.size(); i++) {
                PushStringToTable(L, i, allThingData[i].name);
            }
            dumpstack(L);
            if (CheckLua(L, lua_pcall(L, 1, 0, 0))) {
                cout << "MADE IT" << endl;
            }
        }
    }
    lua_close(L);
}