#include "MapBuilder.h"

MapBuilder *MapBuilder::mapBuilder = nullptr;

MapBuilder::MapBuilder(string sceneName, lua_State* L) : selectedSprite(-1) {
    mapBuilder = this;
    cross = nullptr;
    spriteEditor = nullptr;
    rayEditor = nullptr;
    thingRouter = nullptr;
    followThing = nullptr;

    scene = new Scene(sceneName, L);
    scene->Load(true);
    currentThing = dotThing = scene->addThing(RealThingData(Point(600,600), "EditorDot"));
    scene->EnterLoaded(currentThing);

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
            helpText->setText("Free Move " + to_string(currentThing->position.x) + " : " + to_string(currentThing->position.y));
            focusDot();
            Sprite::removeHighlight();
            state = EditorState::freeMove;
            break;
        case EditorState::play:
            helpText->setText("Play");
            currentThing = scene->addThing(RealThingData(dotThing->position, "testPlayer", "zinnia"), ThingType::fieldPlayer);
            // FOLLOW TESTING
            followThing = scene->spawn("followZinnia", Point(currentThing->position.x + 20, currentThing->position.y + 20));
            // END FOLLOW TESTING
            FocusTracker::ftracker->setFocus(currentThing);
            state = EditorState::play;
            break;
        case EditorState::commandInput:
            helpText->setText("");
            CommandLine::refresh({"play", "new thing", "free", "save", "print things"}, CLIMode::typeCommand);
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
            Scene::currentScene->destroyThing(followThing);
            followThing = nullptr;
            Scene::currentScene->destroyThing(currentThing);
            currentThing = dotThing;
            changeState(EditorState::freeMove);
            return;
        }
    }

    if (state == EditorState::freeMove) {
        helpText->setText("Free Move " + to_string(currentThing->position.x) + " : " + to_string(currentThing->position.y));
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
        if (input == "print things") {
            print();
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
    lua_State* L = scene->L;

    if (!CheckLua(L, luaL_dofile(L, "scripts/save.lua"))) {
        return;
    }
    lua_getglobal(L, "saveMap");
    if (!lua_isfunction(L, -1)) {
        cout << "FAILED TO SAVE! saveMap is not a function!" << endl;
        return;
    }
    lua_newtable(L);
    PushTableToTable(L, "things");
    int thingIndex = 0;
    for (auto const& [i, t] : scene->things) {
        if (t->isSub)
            continue;
        if (t->name == "EditorDot")
            continue;
        PushTableToTable(L, thingIndex);
        PushIntToTable(L, "x", t->origin.x);
        PushIntToTable(L, "y", t->origin.y);
        PushStringToTable(L, "name", t->getBaseName());
        thingIndex += 1;
        lua_pop(L, 1);
    }
    lua_pop(L, 1);
    PushStringToTable(L, "bgTextureName", Camera::c->bgTextureName);
    lua_pushstring(L, Scene::currentScene->sceneName.c_str());
    if (CheckLua(L, lua_pcall(L, 2, 0, 0))) {
        cout << "SAVED" << endl;
    }
}

void MapBuilder::print() {
    vector<RealThingData> allThingData = Scene::currentScene->getAllThingData();
    lua_State* L = scene->L;

    if (!CheckLua(L, luaL_dofile(L, "scripts/save.lua"))) {
        return;
    }
    lua_getglobal(L, "printMap");
    if (!lua_isfunction(L, -1)) {
        cout << "FAILED TO SAVE! save is not a function!" << endl;
        return;
    }
    lua_newtable(L);
    int thingIndex = 0;
    for (auto const& [i, t] : scene->things) {
        if (t->isSub)
            continue;
        if (t->name == "EditorDot")
            continue;
        lua_pushstring(L, t->getBaseName().c_str());
        t->PushThingDataOnStack();
        lua_settable(L, -3);
        thingIndex += 1;
    }
    lua_pushstring(L, Scene::currentScene->sceneName.c_str());
    if (CheckLua(L, lua_pcall(L, 2, 0, 0))) {
        cout << "SAVED" << endl;
    }
}