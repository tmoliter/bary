#include "Task.h"

Subtask::Subtask(lua_State* L,  map<string, RealThing*>& things) : L(L), timer(nullptr), things(things) {
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

void MoveST::init() {
    string thingName;
    if (!luaUtils::GetLuaStringFromTable(L, "thingName", thingName)) {
        return;
    }
    movingThing = things.at(thingName);
    movingThing->AddMove(MoveType::automatic);
}

bool MoveST::meat(KeyPresses keysDown) {
    return true;
    // if (phrase == nullptr)
    //     return true;
    // if(phrase->isComplete())
    //     return true;
    // if (timer) {
    //     if (Subtask::meat(keysDown))
    //         phrase->advance();
    //     return false;
    // }
    // if (keysDown.ok)
    //     phrase->advance();
    // return false;
}

int Task::meat(KeyPresses keysDown) {
    vector<Subtask*> subtasksToDelete;
    for (auto s : subtasks) {
        if (s->meat(keysDown))
            subtasksToDelete.push_back(s);
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

    vector<string> subtaskTypes;
    lua_pushnil(L);
    while (lua_next(L, -2)) {
        if (!lua_isstring(L, -1)) {
            cout << "cannot resolve subtask type to string" << endl;
            continue;
        }
        subtaskTypes.push_back(lua_tostring(L, -1));
        lua_pop(L,1);
    }
    lua_pop(L,1);
    for (auto t : subtaskTypes) {
        if (t == "move")
            subtasks.push_back(new MoveST(L, things));
        if (t == "phrase")
            subtasks.push_back(new PhraseST(L, things));
        if (t == "wait")
            subtasks.push_back(new Subtask(L, things));
        subtasks.back()->init();
        lua_pop(L,1);
    }
}