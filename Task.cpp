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
    phrase = nullptr;
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
    luaUtils::GetLuaStringFromTable(L, "scrollType", scrollType);
    luaUtils::GetLuaIntFromTable(L, "gridLimitsX", gridLimits.x);
    luaUtils::GetLuaIntFromTable(L, "gridLimitsY", gridLimits.y);
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

int Task::meat(KeyPresses keysDown) {
    vector<Subtask*> subtasksToDelete;
    for (auto s : subtasks) {
        if (s->meat(keysDown)) {
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
        if (currentType == "move")
            subtasks.push_back(new MoveST(L, host));
        if (currentType == "phrase")
            subtasks.push_back(new PhraseST(L, host));
        if (currentType == "wait")
            subtasks.push_back(new Subtask(L, host));
        if (currentType == "pauseMoves") {
            pauseMoves(L);
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