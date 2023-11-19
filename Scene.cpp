#include "Scene.h"

Scene::Scene(string sceneName) : sceneName(sceneName) {
    L = luaL_newstate();
    luaL_openlibs(L);
    

    lua_register(L, "_loadScene", _loadScene);
    lua_register(L, "_createThing", _createThing);
    lua_register(L, "_updateMoveTarget", _updateMoveTarget);
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

string Scene::renameThing(RealThing* thing, string newName) {
    // This should only happen in the editor so maybe we can hoist this
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

RealThing* Scene::addExistingThingToScene(RealThing* existingThing) {
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


Animator* Scene::AddAnimator(string name) {
    // Right now there is only one type of animator, but this could take
    // AnimationType in the future
    if (things.count(name) < 1) {
        cout << "can't animate '" << name << "', it isn't a thing";
        return nullptr;
    }
    RealThing* thing = things[name];
    if (thing->sprites.size() != 1) {
        std::cout << "Can't animate no sprites!" << std::endl;
        return nullptr;
    }
    thing->animator = new Animator(thing->sprites[0]);
    thing->animator->splitSheet(9, 4); // Obviously this shouldn't be hard-coded, but for now it is
    thing->bounds.top = 0 - thing->sprites[0]->d.width;
    thing->bounds.bottom = 0;
    thing->bounds.right = 0 - ( thing->sprites[0]->d.width / 2); // I think these are backwards
    thing->bounds.left = (thing->sprites[0]->d.width / 2);
    animatedThings[name] = thing;
    return thing->animator;
}

Move* Scene::AddMove(string name, MoveType type) { // this should just take a pointer I think
    if (things.count(name) < 1) {
        cout << "can't make '" << name << "' move, it isn't a thing";
        return nullptr;
    }
    RealThing* thing = things[name];

    thing->move = new Move(type, thing->position);

    Bounds bounds = thing->bounds;
    vector<Ray> obstructionRays = {
        Ray(Point(bounds.left - 10, bounds.bottom), Point(bounds.right + 8, bounds.bottom)),
        Ray(Point(bounds.right + 8, bounds.bottom), Point(bounds.right + 8, bounds.bottom - 6)),
        Ray( Point(bounds.right + 8, bounds.bottom - 6), Point(bounds.left - 10, bounds.bottom - 6)),
        Ray(Point(bounds.left - 10, bounds.bottom - 6), Point(bounds.left - 10, bounds.bottom))
    };
    thing->addObstruction(obstructionRays, 0);
    movinThings[name] = thing;
    return thing->move;
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
    newThing->sceneL = L;
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
            AddMove(thing->name, MoveType::automatic);
        if (component == "followMove")
            thing->move->type = MoveType::follow;
        if (component == "moveAnimate")
            AddAnimator(thing->name);
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
    if(!CheckParams(L, {ParamType::point, ParamType::table, ParamType::str})) {
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
    if(!CheckParams(L, {ParamType::point, ParamType::table, ParamType::table })) {
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
    if(!CheckParams(L, {ParamType::point, ParamType::number, ParamType::number}))
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