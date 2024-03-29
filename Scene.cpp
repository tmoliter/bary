#include "Scene.h"

Scene::Scene(string sceneName, lua_State *L) : sceneName(sceneName) {
    this->L = L;
    lua_register(L, "_loadScene", _loadScene);
    lua_register(L, "_createThing", _createThing);
    lua_register(L, "_newTask", _newTask);
    lua_register(L, "_getThingData", RealThing::_getThingData);
}

Scene::~Scene() {
    destroyAllThings();
    resourceDepository::releaseTexture(bgTextureName);
    resourceDepository::removeUnreferencedTextures(); // should revisit what this does
}

void Scene::Load(bool isEditing) {
    RealThingData sceneManagerTD = RealThingData();
    sceneManagerTD.name = "sceneManager";
    sceneManager = addThing(sceneManagerTD);
    loadLuaFunc("loadScene");
    lua_pushstring(L, sceneName.c_str());
    lua_pushboolean(L, isEditing);
    lua_pushlightuserdata(L, sceneManager);
    cout << "Loading scene..." << endl;
    callLuaFunc(3, 0, 0);
}

void Scene::EnterLoaded(RealThing* focus) {
    new Camera();
    Camera::c->bgTextureName = bgTextureName;

    Camera::c->init();
    new FocusTracker(focus);
    Scene::currentScene = this;
}

string Scene::getNewThingName(string name) {
    name.erase(std::remove_if(name.begin(), name.end(), ::isspace), name.end()); // remove whitespace
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
            newThing = new FieldPlayer(tD, things);
            break;
        case ThingType::thing:
        default:
            newThing = new RealThing(tD, things);
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
        delete thing->animator;
    }
    if (thing->move != nullptr) {
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
    meatThings(keysDown, activeTasks.size() > 0 && meatEvent(keysDown));
}


bool Scene::meatEvent(KeyPresses keysDown) {
    vector<Task*> tasksToDelete;
    map<pair<Host*, string>, int> eventsToResume;
    bool blocking = false;
    for (int i = activeTasks.size() - 1; i >= 0; i--) {
        Task* t = activeTasks[i];
        pair<Host*, string> hostEventName = make_pair(t->host, t->eventName);
        if (!blocking && t->blocking)
            blocking = true;
        if (t->meat(keysDown) < 1) {
            // task has exhausted subtasks
            if (!eventsToResume.count(hostEventName))
                eventsToResume[hostEventName] = eventArgKeys.at(hostEventName);
            tasksToDelete.push_back(t);
        } else {
            if (!eventsToResume.count(hostEventName))
                eventsToResume[hostEventName] = LUA_NOREF;
            else
                eventsToResume.at(hostEventName) = LUA_NOREF;
        }
        if (blocking)
            break;
    }
    for (auto t : tasksToDelete) {
        delete t;
        activeTasks.erase(remove(activeTasks.begin(), activeTasks.end(), t), activeTasks.end());
    }
    for (auto e : eventsToResume) {
        if (e.second == LUA_NOREF)
            continue;
        eventArgKeys.erase(e.first);
        // All tasks for this event have exhausted subtasks, so we look for more tasks
        loadLuaFunc("resumeEvent", e.first.first);
        lua_geti(L, LUA_REGISTRYINDEX, e.second);
        luaUtils::PushStringToTable(L, "eventName", e.first.second);
        callLuaFunc(1, 1, 0);
        luaL_unref(L, LUA_REGISTRYINDEX, e.second);
    }
    return blocking;
}


void Scene::meatThings(KeyPresses keysDown, bool blockingEvent) {
    if (sceneState == SceneState::pauseThings)
        return;
    if (sceneState == SceneState::pausePlayerControl || blockingEvent)
        keysDown = KeyPresses();
    for (auto const& [id, thing] : things)
        thing->processMove(keysDown);
    for (auto const& [id, thing] : things)
        thing->processCollisions(things);
    for (auto const& [id, thing] : things)
        thing->meat(keysDown);
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

RealThing* Scene::spawn(string baseName, Point position) {
    loadLuaFunc("spawn");
    lua_pushstring(L, sceneName.c_str());
    lua_newtable(L);
    luaUtils::PushStringToTable(L, "baseName", baseName);
    luaUtils::PushIntToTable(L, "x", position.x);
    luaUtils::PushIntToTable(L, "y", position.y);
    callLuaFunc(2,1,0);
    if (!lua_islightuserdata(L, -1)) {
        cout << "spawn did not return a thing" << endl;
        throw exception();
    }
    RealThing* spawnedThing = static_cast<RealThing*>(lua_touserdata(L,-1));
    lua_settop(L, 0);
    return spawnedThing;
}

RealThing* Scene::buildThingFromTable() {
    RealThingData td;
    GetLuaIntFromTable(L, "x", td.x);
    GetLuaIntFromTable(L, "y", td.y);
    GetLuaStringFromTable(L, "name", td.name);

    GetTableOnStackFromTable(L, "spriteDataVector");
    lua_pushnil(L);
    while (lua_next(L, -2)) {
        td.spriteDataVector.push_back(SpriteData());
        SpriteData &newSpriteData = td.spriteDataVector.back();
        if (!GetLuaBoolFromTable(L, "active", newSpriteData.active))
            newSpriteData.active = true;
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
    lua_pop(L, 1);

    GetTableOnStackFromTable(L, "obstructionData");
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

    RealThing* newThing;
    bool isFieldPlayer = CheckLuaTableForBool(L, "fieldPlayer");
    if (isFieldPlayer)
        newThing = addThing(td, ThingType::fieldPlayer);
    else
        newThing = addThing(td);

    if (GetTableOnStackFromTable(L, "subThings")) {
        lua_pushnil(L);
        while (lua_next(L, -2)) {
            RealThing* subThing = buildThingFromTable();
            subThing->isSub = true;
            subThing->position.x += td.x;
            subThing->position.y += td.y;
            newThing->subThings.push_back(subThing);
        }
        lua_pop(L, 1);
    }

    if (GetTableOnStackFromTable(L, "components")) {
        newThing->addComponentsFromTable();
    }

    lua_pop(L, 1);
    return newThing;
}

vector<RealThingData> Scene::getAllThingData() {
    vector<RealThingData> allData;
    for (auto const& [i, t] : things) {
        if (t->name == "EditorDot")
            continue;
        if (t->name == "testPlayer")
            continue;
        allData.push_back(t->getData());
    }
    return allData;
}

// Lua registered functions
int Scene::_loadScene(lua_State* L) {
    if(!CheckParams(L, {ParamType::pointer, ParamType::table, ParamType::table, ParamType::str})) {
        cout << "_loadScene failed!" << endl;
        throw exception();
    }
    Scene* scene = static_cast<Scene*>(lua_touserdata(L, -1));
    lua_pop(L, 1);
    resourceDepository::loadScene(L);
    lua_pop(L, 1);
    lua_pushnil(L);
    while (lua_next(L, -2))
        scene->buildThingFromTable();
    lua_pop(L,1);
    scene->bgTextureName = lua_tostring(L, -1);
    lua_settop(L, 0);
    cout << "Scene Loaded!" << endl;
    return 0;
}

int Scene::_createThing(lua_State* L) {
    if(!CheckParams(L, {ParamType::pointer, ParamType::table })) {
        cout << "_createThing failed!" << endl;
        throw exception();
    }
    Scene* scene = static_cast<Scene*>(lua_touserdata(L, -1));
    lua_pop(L, 1);
    RealThing* newThing = scene->buildThingFromTable();

    lua_pushlightuserdata(L, newThing);
    return 1;
}

int Scene::_newTask(lua_State *L) {
    if(!CheckParams(L, {ParamType::pointer, ParamType::str, ParamType::table })) {
        cout << "_newTask failed!" << endl;
        throw exception();
    }
    Host* host = static_cast<Host*>(lua_touserdata(L, -1));
    lua_pop(L, 1);

    RealThing* hostThing = static_cast<RealThing*>(host);
    Scene* scene = static_cast<Scene*>(hostThing->parentScene);

    string eventName = lua_tostring(L, -1);
    pair<Host*, string> hostEventName = make_pair(hostThing, eventName);
    if (!scene->eventArgKeys.count(hostEventName)) {
        lua_newtable(host->L);
        scene->eventArgKeys[hostEventName] = luaL_ref(host->L, LUA_REGISTRYINDEX);
    }
    Task* newTask = new Task(eventName, host, scene->eventArgKeys[hostEventName]);
    lua_pop(L, 1);

    lua_xmove(L, host->L, 1);

    newTask->addSubtasks(host->L);

    scene->activeTasks.push_back(newTask);
    lua_settop(L, 0);
    return 0;
}
