#include "Task.h"

Subtask::Subtask(lua_State* L,  Host* host) : L(L), timer(nullptr), host(host) {
    if (!lua_istable(L, -1)) {
        throw exception();
    }
    if(luaUtils::GetLuaIntFromTable(L, "frames", framesToWait)) {
        timer = new Timer();
    }
}

bool Subtask::meat(KeyPresses keysDown) {
    if (timer)
        return timer->getTime() >= framesToWait;
    return false;
}

Subtask::~Subtask() {
    if (timer)
        delete timer;
}

PhraseST::~PhraseST() {
    UIRenderer::removePhrase(phrase);
}

void PhraseST::init() {
    string text;
    Point point;
    Point size;
    string scrollType;
    Point gridLimits;
    luaUtils::GetLuaStringFromTable(L, "text", text);
    luaUtils::GetLuaIntFromTable(L, "x", point.x);
    luaUtils::GetLuaIntFromTable(L, "y", point.y);
    luaUtils::GetLuaIntFromTable(L, "width", size.x);
    luaUtils::GetLuaIntFromTable(L, "height", size.y);
    if (!luaUtils::GetLuaStringFromTable(L, "scrollType", scrollType))
        scrollType = "allButLast";
    if (!luaUtils::GetLuaIntFromTable(L, "gridLimitsX", gridLimits.x))
        gridLimits.x = 1000;
    if (!luaUtils::GetLuaIntFromTable(L, "gridLimitsY", gridLimits.y))
        gridLimits.y = 1000;
    phrase = new Phrase(point, size, ScrollType::allButLast, text, gridLimits);
    UIRenderer::addPhrase(phrase);
}

bool PhraseST::meat(KeyPresses keysDown) {
    if (phrase == nullptr)
        return true;
    if(phrase->isComplete())
        return true;
    if (timer) {
        if (Subtask::meat(keysDown))
            phrase->advance();
        return false;
    }
    if (keysDown.ok)
        phrase->advance();
    return false;
}

MenuST::~MenuST() {
    if (closeOnDestroy) {
        UIRenderer::removeMenuDisplay(menu);
    }
}

void MenuST::init() {
    vector<Option> options;
    Point point;
    Point size;
    int maxColumns;
    if (luaUtils::GetTableOnStackFromTable(L, "options")) {
        lua_pushnil(L);
        while (lua_next(L, -2)) {
            Option newOption = Option();
            luaUtils::GetLuaStringFromTable(L, "selectionText", newOption.selectionText);
            luaUtils::GetLuaStringFromTable(L, "flavorText", newOption.flavorText);
            luaUtils::GetLuaStringFromTable(L, "value", newOption.value);
            options.push_back(newOption);
            lua_pop(L,1);
        }
        lua_pop(L,1);
    }
    luaUtils::GetLuaIntFromTable(L, "x", point.x);
    luaUtils::GetLuaIntFromTable(L, "y", point.y);
    luaUtils::GetLuaIntFromTable(L, "width", size.x);
    luaUtils::GetLuaIntFromTable(L, "height", size.y);
    luaUtils::GetLuaIntFromTable(L, "maxColumns", maxColumns);
    luaUtils::GetLuaBoolFromTable(L, "closeOnDestroy", closeOnDestroy);
    menu = new MenuDisplay(options, point, size, maxColumns);
    menu->addBox("pinkbox", {0, 0, 340, 120});
    selection = menu->getCurrentSelection().value; // This way of doing things could probably be improved
    UIRenderer::addMenuDisplay(menu);
}

bool MenuST::meat(KeyPresses keysDown) {
    if (menu->processInput(keysDown, selection))
        return true;
    return false;
}

bool MenuST::pushArgs() { 
    luaUtils::PushStringToTable(L, "selection", selection);
    return true; 
};

MoveST::~MoveST() {
    if (movingThing == nullptr)
        return;
    if (prevMove != nullptr)
        prevMove->disables--;
    movingThing->move = prevMove;
}

void MoveST::init() {
    string thingName;
    Point destination;
    Point offset;
    RealThing* hostThing = static_cast<RealThing*>(host); // (assume-host)
    if (luaUtils::GetLuaStringFromTable(L, "thingName", thingName)) {
        movingThing = hostThing->things.at(thingName);
    } else {
        movingThing = hostThing;
    }
    if (!luaUtils::GetLuaIntFromTable(L, "destinationX", destination.x))
        destination.x = movingThing->position.x;
    if (!luaUtils::GetLuaIntFromTable(L, "destinationY", destination.y))
        destination.y = movingThing->position.y;
    if (!luaUtils::GetLuaIntFromTable(L, "offsetX", offset.x))
        offset.x = 0;
    if (!luaUtils::GetLuaIntFromTable(L, "offsetY", offset.y))
        offset.y = 0;

    prevMove = movingThing->move;
    if (prevMove != nullptr)
        prevMove->disables++;
    movingThing->AddMove(MoveType::automatic)->destination = addPoints(destination, offset);
}

bool MoveST::meat(KeyPresses keysDown) {
    if (movingThing == nullptr)
        return true;
    return movingThing->position.isWithin(movingThing->move->destination, movingThing->move->tolerance); // break out into move function I think
}

void PortalST::init() {
    Host* incomingThing;
    string thingName = "";
    RealThing* hostThing = static_cast<RealThing*>(host); // (assume-host)
    if (Host::GetLuaHostFromTable(L, "thing", incomingThing))
        thing = static_cast<RealThing*>(incomingThing);
    else if (luaUtils::GetLuaStringFromTable(L, "thingName", thingName))
        thing = hostThing->things.at(thingName);
    else
        thing = hostThing;

    luaUtils::GetLuaIntFromTable(L, "newLayer", newLayer);
    Point relativeMove;
    if (!luaUtils::GetLuaIntFromTable(L, "relativeX", relativeMove.x) ||
        !luaUtils::GetLuaIntFromTable(L, "relativeY", relativeMove.y)) {
        Point offset;
        if (!luaUtils::GetLuaIntFromTable(L, "destinationX", destination.x))
            destination.x = thing->position.x;
        if (!luaUtils::GetLuaIntFromTable(L, "destinationY", destination.y))
            destination.y = thing->position.y;
        if (!luaUtils::GetLuaIntFromTable(L, "offsetX", offset.x))
            offset.x = 0;
        if (!luaUtils::GetLuaIntFromTable(L, "offsetY", offset.y))
            offset.y = 0;
        destination = addPoints(destination, offset);
    } else {
        Point hostPosition = static_cast<RealThing*>(host)->position;
        destination = addPoints(hostPosition, relativeMove);
    }

    Camera::fadeOut(3);
    for (auto const& [id, t] : thing->things) {
        if (!t->move)
            continue;
        t->move->disables += 1;
    }
}

PortalST::~PortalST() {
    for (auto const& [id, t] : thing->things) {
        if (!t->move)
            continue;
        t->move->disables -= 1;
    }
}

bool PortalST::meat(KeyPresses keysDown) {
    if (Camera::c->fadeStatus == FxStatus::applied) {
        thing->position = destination;
        thing->shiftLayer(newLayer);
        Camera::c->fadeIn(3);
        return 0;
    }
    if (Camera::c->fadeStatus == FxStatus::unapplied)
        return 1;
    return 0;
}

int Task::meat(KeyPresses keysDown) {
    vector<Subtask*> subtasksToDelete;
    for (auto s : subtasks) {
        if (s->meat(keysDown)) {
            lua_geti(host->L, LUA_REGISTRYINDEX, argKey);
            s->pushArgs();
            lua_pop(host->L, 1);
            subtasksToDelete.push_back(s);
        }
    }
    for (auto s : subtasksToDelete) {
        delete s;
        subtasks.erase(remove(subtasks.begin(), subtasks.end(), s), subtasks.end());
    }
    return subtasks.size();
}

void Task::addSubtasks(lua_State* L) {
    if (!lua_istable(L, -1))
        luaUtils::ThrowLua(L, "top of stack is not list of subTasks!");

    lua_pushnil(L);
    string currentType;
    while (lua_next(L, -2)) {
        if (!lua_istable(L, -1)) {
            cout << "subtask should be a table" << endl;
            lua_pop(L, 1);
            continue;
        }
        if(!luaUtils::GetLuaStringFromTable(L, "type", currentType)) {
            cout << "subtask has no type!" << endl;
            lua_pop(L, 1);
            continue;
        }
        bool instant = false;
        if (!blocking)
            luaUtils::GetLuaBoolFromTable(L, "blocking", blocking);
        if (currentType == "wait")
            subtasks.push_back(new Subtask(L, host));
        if (currentType == "phrase")
            subtasks.push_back(new PhraseST(L, host));
        if (currentType == "menu")
            subtasks.push_back(new MenuST(L, host));
        if (currentType == "move")
            subtasks.push_back(new MoveST(L, host));
        if (currentType == "portal")
            subtasks.push_back(new PortalST(L, host));
        if (currentType == "fireEvent") {
            luaUtils::GetTableOnStackFromTable(L, "args");
            int ref = luaL_ref(L, LUA_REGISTRYINDEX);
            host->loadLuaFunc("beginEvent");
            lua_geti(L, LUA_REGISTRYINDEX, ref);
            host->callLuaFunc(1, 0, 0);
            luaL_unref(L, LUA_REGISTRYINDEX, ref);
            instant = true;
        }
        if (currentType == "pauseMoves") {
            pauseMoves(L);
            instant = true;
        }
        if (currentType == "setActiveSprites") {
            RealThing* hostThing = static_cast<RealThing*>(host);
            for (auto s : hostThing->sprites)
                s->active = false;
            if (luaUtils::GetTableOnStackFromTable(L, "sprites")) {
                lua_pushnil(L);
                while (lua_next(L, -2)) {
                    if (!lua_isnumber(L, -1)) {
                        cout << "all list sprite indices should be numbers" << endl;
                        throw exception();
                    }
                    hostThing->sprites[lua_tonumber(L, -1)]->active = true;
                    lua_pop(L,1);
                }
                lua_pop(L,1);
            }
            instant = true;
        }
        if (currentType == "disableColliders") {
            bool enable = luaUtils::CheckLuaTableForBool(L, "enable");
            bool obs, inters, trigs = luaUtils::CheckLuaTableForBool(L, "all");
            luaUtils::GetLuaBoolFromTable(L, "obstructions", obs);
            luaUtils::GetLuaBoolFromTable(L, "interactables", inters);
            luaUtils::GetLuaBoolFromTable(L, "triggers", trigs);
            RealThing* hostThing = static_cast<RealThing*>(host);
            if (obs)
                for (auto c : hostThing->obstructions)
                    c.second->active = enable;
            if (inters)
                for (auto c : hostThing->interactables)
                    c.second->active = enable;
            if (trigs)
                for (auto c : hostThing->triggers)
                    c.second->active = enable;
            instant = true;
        }
        if (!instant)
            subtasks.back()->init();
        lua_pop(L,1);
    }
    lua_pop(L,1);
}

void Task::pauseMoves(lua_State* L) {
    if (!lua_istable(L, -1)) {
        throw exception();
    }
    RealThing* hostThing = static_cast<RealThing*>(host);
    int disable = luaUtils::CheckLuaTableForBool(L, "unpause") ? -1 : 1;
    if (luaUtils::CheckLuaTableForBool(L, "all")) {
        for (auto const& [id, thing] : hostThing->things) {
            if (!thing->move)
                continue;
            thing->move->disables += disable;
        }
        return;
    }
    set<string> thingNames;
    if (luaUtils::CheckLuaTableForBool(L, "hostThing")) {
        thingNames.insert(hostThing->name);
    }
    if (luaUtils::GetTableOnStackFromTable(L, "thingNames")) {
        lua_pushnil(L);
        while (lua_next(L, -2)) {
            if (!lua_isstring(L, -1)) {
                cout << "all list items should be strings" << endl;
                continue;
            }
            thingNames.insert(lua_tostring(L, -1));
            lua_pop(L,1);
        }
        lua_pop(L,1);
    }
    for (auto thingName : thingNames) {
        if (!hostThing->things.count(thingName))
            continue;
        RealThing* thing = hostThing->things.at(thingName);
        if (!thing->move)
            continue;
        thing->move->disables += disable;
    }
}