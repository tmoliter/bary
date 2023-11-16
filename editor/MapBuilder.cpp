#include "MapBuilder.h"

MapBuilder *MapBuilder::mapBuilder = nullptr;

MapBuilder::MapBuilder() : selectedSprite(-1) {
    mapBuilder = this;
    cross = nullptr;
    spriteEditor = nullptr;
    rayEditor = nullptr;
    eventEditor = nullptr;
    thingRouter = nullptr;

    scene = new Scene("Burg");
    currentThing = dotThing = scene->addThing(Point(600,600), "EditorDot");
    FocusTracker::ftracker->setFocus(currentThing);

    CommandLine::init();

    dotThing->AddRawSprite("singlepixel");

    // Maybe we can pass this down into Editors to share? Maybe that's dumb.
    helpText = new Text(Point(16, 16), "");
    UIRenderer::addText(helpText);
    Scene::currentScene->showAllLines();

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
            FocusTracker::ftracker->setFocus(currentThing);
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
    FocusTracker::ftracker->setFocus(currentThing);
    if (cross == nullptr)
        return;
    delete cross;
    cross = nullptr;
}

void MapBuilder::updateLines() {
    if(currentThing == dotThing)
        Scene::currentScene->showAllLines();
    else {
        Scene::currentScene->hideAllLines();
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
            Scene::currentScene->destroyThing(currentThing);
            currentThing = dotThing;
            changeState(EditorState::freeMove);
            return;
        }
    }

    if (state == EditorState::freeMove) {
        if (keysDown.ok) {
            vector<RealThing*> collisions = Scene::currentScene->findThingsByPoint(dotThing->position);
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
    vector<RealThingData> allThingData = Scene::currentScene->getAllThingData();
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    if (CheckLua(L, luaL_dofile(L, "scripts/save.lua"))) {
        lua_getglobal(L, "dump");
        if (lua_isfunction(L, -1)) {
            lua_newtable(L);
            for (int i = 0; i < allThingData.size(); i++) {
                PushTableToTable(L, i);
                PushStringToTable(L, "name", allThingData[i].name);
                PushIntToTable(L, "x", allThingData[i].x);
                PushIntToTable(L, "y", allThingData[i].y);
                PushTableToTable(L, "spriteDataVector");
                for (int j = 0; j < allThingData[i].spriteDataVector.size(); j++) {
                    if (!PushTableToTable(L, j)) {
                        cout << "Failed!" << endl;
                        continue;
                    };
                    PushIntToTable(L, "layer", allThingData[i].spriteDataVector[j].layer);
                    PushIntToTable(L, "renderOffset", allThingData[i].spriteDataVector[j].renderOffset);
                    PushIntToTable(L, "xOffset", allThingData[i].spriteDataVector[j].xOffset);
                    PushIntToTable(L, "yOffset", allThingData[i].spriteDataVector[j].yOffset);
                    PushIntToTable(L, "sourceX", allThingData[i].spriteDataVector[j].sourceX);
                    PushIntToTable(L, "sourceY", allThingData[i].spriteDataVector[j].sourceY);
                    PushIntToTable(L, "width", allThingData[i].spriteDataVector[j].width);
                    PushIntToTable(L, "height", allThingData[i].spriteDataVector[j].height);
                    PushStringToTable(L, "textureName", allThingData[i].spriteDataVector[j].textureName);
                    lua_pop(L, 1);
                }
                lua_pop(L, 1);
                PushTableToTable(L, "obstructionData");
                for (int j = 0; j < allThingData[i].obstructionData.size(); j++) {
                    if (!PushTableToTable(L, j)) {
                        cout << "Failed!" << endl;
                        continue;
                    };
                    PushIntToTable(L, "layer", allThingData[i].obstructionData[j].layer);
                    PushTableToTable(L, "rays");
                    for (int h = 0; h < allThingData[i].obstructionData[j].rays.size(); h++) {
                        if (!PushTableToTable(L, h)) {
                            cout << "Failed!" << endl;
                            continue;
                        };
                        PushIntToTable(L, "aX", allThingData[i].obstructionData[j].rays[h].a.x);
                        PushIntToTable(L, "aY", allThingData[i].obstructionData[j].rays[h].a.y);
                        PushIntToTable(L, "bX", allThingData[i].obstructionData[j].rays[h].b.x);
                        PushIntToTable(L, "bY", allThingData[i].obstructionData[j].rays[h].b.y);
                        lua_pop(L, 1);
                    }
                    lua_pop(L, 2);
                }
                lua_pop(L, 2);
            }
            if (CheckLua(L, lua_pcall(L, 1, 0, 0))) {
                cout << "SAVED" << endl;
            }
        }
    }
    lua_close(L);
}