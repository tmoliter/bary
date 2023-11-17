#include "Scene.h"

int Scene::_loadScene(lua_State* L) {
    if (lua_gettop(L) != 3)
        throw exception();
    Scene* scene = static_cast<Scene*>(lua_touserdata(L, -1));
    lua_pop(L, 1);
    lua_pushnil(L);
    while (lua_next(L, -2))
        scene->buildThingFromGlobal(L);
    lua_pop(L,1);
    scene->backgroundPath = lua_tostring(L, -1);
    lua_settop(L, 0);
    cout << "Scene Loaded!" << endl;
    return 0;
}

Scene::Scene(string sceneName) : sceneName(sceneName) {
    L = luaL_newstate();
    luaL_openlibs(L);

    lua_register(L, "_loadScene", _loadScene);
}

Scene::~Scene() {
    lua_close(L);
}

void Scene::Load() {
    cout << "Loading scene..." << endl;
    if (!CheckLua(L, luaL_dofile(L, "scripts/load.lua")))
        throw exception();
    lua_getglobal(L, "loadScene");
    if (!lua_isfunction(L, -1)) {
        cout << "loadScene isn't a function!" << endl;
        throw exception();
    }
    lua_pushstring(L, sceneName.c_str());
    lua_pushlightuserdata(L, this);
    if (!CheckLua(L, lua_pcall(L, 2, 0, 0)))
        throw exception();
}

void Scene::EnterLoaded(RealThing* focus) {
    new Camera(renderer);
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

string Scene::renameThing(RealThing* thing, string newName) {
    things.erase(thing->name);
    thing->name = getNewThingName(newName);
    things[thing->name] = thing;
    return thing->name;
}

RealThing* Scene::addThing(RealThingData tD) {
    tD.name = getNewThingName(tD.name);
    RealThing* newThing = new RealThing(tD);
    things[newThing->name] = newThing;
    return newThing;
}

RealThing* Scene::addThing(Point p, string n) {
    string name = getNewThingName(n);
    RealThing* newThing = new RealThing(p, name);
    things[newThing->name] = newThing;
    return newThing;
}

RealThing* Scene::addThingToScene(RealThing* existingThing) {
    existingThing->name = getNewThingName(existingThing->name);
    things[existingThing->name] = existingThing;
    return existingThing;
}

RealThing* Scene::copyThing(RealThing& oldThing) {
    RealThing* newThing = oldThing.copyInPlace();
    newThing->name = getNewThingName(oldThing.name);
    things[newThing->name] = newThing;
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


void Scene::meatThings(KeyPresses keysDown) {
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

int Scene::checkAllInteractables (Ray incoming, int incomingLayer) {
    for (auto const& [name, t] : things) {
        if (t->checkForCollidables(incoming, incomingLayer, CollidableType::interactable))
            return 1;
    }
    return 0;
}

int Scene::checkAllTriggers (Ray incoming, int incomingLayer) {
    for (auto const& [name, t] : things) {
        if (t->checkForCollidables(incoming, incomingLayer, CollidableType::trigger))
            return 1;
    }
    return 0;
}


RealThing* Scene::findRealThing (string name) {
    return things[name];
}

void Scene::buildThingFromGlobal(lua_State* L) {
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
    addThing(td);
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