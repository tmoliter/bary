#include "Scene.h"

Scene::Scene(string sceneName) : sceneName(sceneName) {
    L = luaL_newstate();
    luaL_openlibs(L);

    lua_register(L, "_loadScene", _loadScene);
    lua_register(L, "_createThing", _createThing);
    lua_register(L, "_updateMoveTarget", _updateMoveTarget);
    lua_register(L, "_newTask", _newTask);
}

Scene::~Scene() {
    destroyAllThings();
    lua_close(L);
}

void Scene::Load() {
    cout << "Loading scene..." << endl;
    if (!CheckLua(L, luaL_dofile(L, "scripts/load.lua")))
        throw exception();
    loadLuaFunc(L, "loadScene");
    lua_pushstring(L, sceneName.c_str());
    callLuaFunc(L, 1, 0, 0);
}

void Scene::EnterLoaded(RealThing* focus) {
    new Camera();
    Camera::c->path = backgroundPath;

    Camera::c->init();
    new FocusTracker(focus);
    Scene::currentScene = this;
}

string Scene::getNewThingName(string name) {
    int i;
    for (i = 0; i < name.length(); i++)
        if (isdigit(name[i]))
            break;
    string baseName = name.substr(0, i);
    i = 1;
    name = baseName;
    while(things.count(name)) {
        name = baseName + to_string(i);
        i++;
    }
    return name;
}

RealThing::ThingLists Scene::getThingLists() {
    return RealThing::ThingLists(
        things,
        movinThings,
        animatedThings
    );
}

string Scene::renameThing(RealThing* thing, string newName) {
    // This should only happen in the editor so maybe we can hoist this
    things.erase(thing->name);
    thing->name = getNewThingName(newName);
    thing->AddToMap(things);
    return thing->name;
}

RealThing* Scene::addThing(RealThingData tD, ThingType type) {
    tD.name = getNewThingName(tD.name);
    RealThing* newThing;
    switch(type) {
        case ThingType::fieldPlayer:
            newThing = new FieldPlayer(tD, getThingLists());
            break;
        case ThingType::door:
            newThing = new Door(tD, getThingLists());
            break;
        case ThingType::thing:
        default:
            newThing = new RealThing(tD, getThingLists());
            break;
    }
    newThing->AddToMap(things);
    newThing->parentScene = this;
    newThing->L = L;
    return newThing;
}

RealThing* Scene::addExistingThingToScene(RealThing* existingThing) {
    existingThing->name = getNewThingName(existingThing->name);
    existingThing->AddToMap(things);
    return existingThing;
}

RealThing* Scene::copyThing(RealThing& oldThing) {
    RealThing* newThing = oldThing.copyInPlace();
    newThing->name = getNewThingName(oldThing.name);
    newThing->AddToMap(things);
    return newThing;
}

void Scene::destroyThing(RealThing* thing) {
    for (auto subThing : thing->subThings)
        destroyThing(subThing);
    if (thing->animator != nullptr) {
        animatedThings.erase(thing->name);
        delete thing->animator;
    }
    if (thing->move != nullptr) {
        movinThings.erase(thing->name);
        delete thing->move;
    }
    things.erase(thing->name);
    delete thing;
}

void Scene::addThingToDestroyQueue(string n) {
    thingsToDestroy.push_back(n);
}


void Scene::destroyThings() {
    for (auto killMe : thingsToDestroy) {
        RealThing* thing = things[killMe];
        destroyThing(thing);
    }
    thingsToDestroy.clear();
}

void Scene::destroyAllThings() {
    map<string, RealThing*>::iterator itr = things.begin();
    while (itr != things.end()) {
        delete itr->second;
        itr = things.erase(itr);
    }
}

void Scene::meat(KeyPresses keysDown) {
    if (sceneState == SceneState::pauseAll) {
        // Listen for unpause
        return;
    }
    if (activeTasks.size() > 0) {
        meatEvent(keysDown);
    }
    else
        meatThings(keysDown); // should we actually pause all things or not?
}


void Scene::meatEvent(KeyPresses keysDown) { // Maybe we could return a bool to decide whether to block meat here
    vector<Task*> tasksToDelete;
    map<string, pair<bool, RealThing*>> eventsToResume;
    for (auto t : activeTasks) {
        if (t->meat(keysDown) < 1) {
            // task has exhausted subtasks
            if (!eventsToResume.count(t->eventName))
                eventsToResume[t->eventName] = make_pair(true, t->hostThing);
            tasksToDelete.push_back(t);
            loadLuaFunc(L, "resumeEvent");
            lua_pushlightuserdata(L, t->hostThing);
            lua_pushstring(L, t->eventName.c_str());
            callLuaFunc(L, 2, 1, 0);
        } else {
            if (!eventsToResume.count(t->eventName))
                eventsToResume[t->eventName] = make_pair(false, t->hostThing);
            else
                eventsToResume.at(t->eventName).first = false;
        }
    }
    for (auto e : eventsToResume) {
        if (!e.second.first)
            continue;
        // All tasks for this event have exhausted subtasks, so we look for more tasks
        loadLuaFunc(L, "resumeEvent");
        lua_pushlightuserdata(L, e.second.second);
        lua_pushstring(L, e.first.c_str());
        callLuaFunc(L, 2, 1, 0);
    }
    for (auto t : tasksToDelete) {
        delete t;
        activeTasks.erase(remove(activeTasks.begin(), activeTasks.end(), t), activeTasks.end());
    }
}


void Scene::meatThings(KeyPresses keysDown) {
    if (sceneState == SceneState::pauseThings)
        return;
    if (sceneState == SceneState::pausePlayerControl)
        keysDown = KeyPresses();
    for (auto const& [id, thing] : movinThings){
        thing->processMove(keysDown);
    }
    for (auto const& [id, thing] : movinThings){
        thing->processCollisions(things);
    }
    for (auto const& [id, thing] : things){
        thing->meat(keysDown);
    }
}


vector<RealThing*> Scene::findThingsByPoint(Point p) {
    vector<RealThing*> matches;
    for (auto const& [id, thing] : things) {
        if (pointIsInside(p, thing->position, thing->bounds))
            matches.push_back(thing);
    }
    return matches;
}

void Scene::showAllLines() {
    for (auto const& [id, t] : things)
        t->showLines();
}

void Scene::hideAllLines() {
    for (auto const& [id, t] : things)
        t->hideLines();
}

RealThing* Scene::findRealThing (string name) {
    return things.at(name);
}

RealThing* Scene::buildThingFromTable(lua_State* L) {
    RealThingData td;
    GetLuaIntFromTable(L, "x", td.x);
    GetLuaIntFromTable(L, "y", td.y);
    GetLuaStringFromTable(L, "name", td.name);
    GetTableOnStackFromTable(L, "spriteDataVector");
    if(!lua_isnil(L, -1)) {
        lua_pushnil(L);
        while (lua_next(L, -2)) {
            td.spriteDataVector.push_back(SpriteData());
            SpriteData &newSpriteData = td.spriteDataVector.back();
            GetLuaIntFromTable(L, "height", newSpriteData.height);
            GetLuaIntFromTable(L, "width", newSpriteData.width);
            GetLuaIntFromTable(L, "layer", newSpriteData.layer);
            GetLuaIntFromTable(L, "renderOffset", newSpriteData.renderOffset);
            GetLuaIntFromTable(L, "xOffset", newSpriteData.xOffset);
            GetLuaIntFromTable(L, "yOffset", newSpriteData.yOffset);
            GetLuaIntFromTable(L, "sourceX", newSpriteData.sourceX);
            GetLuaIntFromTable(L, "sourceY", newSpriteData.sourceY);
            GetLuaStringFromTable(L, "textureName", newSpriteData.textureName);
            lua_pop(L, 1);
        }
    }
    lua_pop(L, 1);
    GetTableOnStackFromTable(L, "obstructionData");
    if(!lua_isnil(L, -1)) {
        lua_pushnil(L);
        while (lua_next(L, -2)) {
            td.obstructionData.push_back(CollidableData());
            CollidableData &newObstructionData = td.obstructionData.back();
            GetLuaIntFromTable(L, "layer", newObstructionData.layer);
            GetTableOnStackFromTable(L, "rays");
            lua_pushnil(L);
            while (lua_next(L, -2)) {
                newObstructionData.rays.push_back(Ray());
                Ray& newRay = newObstructionData.rays.back();
                GetLuaIntFromTable(L, "aX", newRay.a.x);
                GetLuaIntFromTable(L, "aY", newRay.a.y);
                GetLuaIntFromTable(L, "bX", newRay.b.x);
                GetLuaIntFromTable(L, "bY", newRay.b.y);
                lua_pop(L, 1);
            }
            lua_pop(L, 2);
        }
        lua_pop(L, 1);
    }
    lua_pop(L, 1);
    RealThing* newThing = addThing(td);
    return newThing;
}

void Scene::addComponentsFromTable(lua_State* L, RealThing* thing) {
    if (!lua_istable(L, -1)) {
        cout << "top of stack is not table of components!" << endl;
        return;
    }
    lua_pushnil(L);
    while (lua_next(L, -2)) {
        if (!lua_isstring(L, -1)) {
            cout << "component name is not string!" << endl;
            continue;
        }
        string component = lua_tostring(L, -1); // we will have more data than just component name eventually
        if (component == "autoMove")
            thing->AddMove(MoveType::automatic);
        if (component == "followMove")
            thing->move->type = MoveType::follow;
        if (component == "moveAnimate")
            thing->AddAnimator();
        if (component == "standardCollider")
            thing->AddStandardCollision();
        lua_pop(L, 1);
    }
    lua_pop(L, 1);
}

vector<RealThingData> Scene::getAllThingData() {
    vector<RealThingData> allData;
    for (auto const& [i, t] : things) {
        if (t->name == "EditorDot")
            continue;
        if (t->name == "test player")
            continue;
        allData.push_back(t->getData());
    }
    return allData;
}

// Lua registered functions
int Scene::_loadScene(lua_State* L) {
    if(!CheckParams(L, {ParamType::pointer, ParamType::table, ParamType::str})) {
        cout << "_loadScene failed!" << endl;
        throw exception();
    }
    Scene* scene = static_cast<Scene*>(lua_touserdata(L, -1));
    lua_pop(L, 1);
    lua_pushnil(L);
    while (lua_next(L, -2))
        scene->buildThingFromTable(L);
    lua_pop(L,1);
    scene->backgroundPath = lua_tostring(L, -1);
    lua_settop(L, 0);
    cout << "Scene Loaded!" << endl;
    return 0;
}

int Scene::_createThing(lua_State* L) {
    if(!CheckParams(L, {ParamType::pointer, ParamType::table, ParamType::table })) {
        cout << "_createThing failed!" << endl;
        throw exception();
    }
    Scene* scene = static_cast<Scene*>(lua_touserdata(L, -1));
    lua_pop(L, 1);
    RealThing* newThing = scene->buildThingFromTable(L);
    scene->addComponentsFromTable(L, newThing);

    lua_pushstring(L, newThing->name.c_str());
    return 1;
}

int Scene::_updateMoveTarget(lua_State *L) {
    if(!CheckParams(L, {ParamType::pointer, ParamType::number, ParamType::number}))
        return 0;
    RealThing* thing = static_cast<RealThing*>(lua_touserdata(L, -1));
    Move* move = thing->move;
    if (move == nullptr) {
        cout << "can't update '" << thing->name << "' move target because it does not move" << endl;
        return 0;
    }
    int newY = lua_tointeger(L, -2);
    int newX = lua_tointeger(L, -3);
    lua_settop(L, 0);
    move->destination = Point(newX, newY);
    return 0;
}

int Scene::_newTask(lua_State *L) {
    if(!lua_islightuserdata(L, -1))
        luaUtils::ThrowLua(L, "top param to _newTask is not a Scene pointer!");
    Scene* scene = static_cast<Scene*>(lua_touserdata(L, -1));
    lua_pop(L, 1);

    if(!lua_islightuserdata(L, -1))
        luaUtils::ThrowLua(L,  "second param to _newTask is not an host Thing!" );
    RealThing* hostThing = static_cast<RealThing*>(lua_touserdata(L, -1)); // should eventually pass this into task
    lua_pop(L, 1);

    if(!lua_isstring(L, -1))
        luaUtils::ThrowLua(L, "third param to _newTask is not an event name!" );
    string eventName = lua_tostring(L, -1);
    Task* newTask = new Task(eventName, hostThing);
    lua_pop(L, 1);

    newTask->addSubtasks(L);
    scene->activeTasks.push_back(newTask);
    lua_settop(L, 0);
    return 0;
}
