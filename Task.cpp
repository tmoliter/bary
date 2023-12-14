#include "Task.h"

Subtask::Subtask(lua_State* L) {
    if (!lua_istable(L, -1)) {
        throw exception();
    }
}

PhraseST::PhraseST(lua_State* L) : Subtask(L) {
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
    if(phrase->isComplete()) {
        cout << "COMPLETE" << endl;
        return true;
    }
    if (keysDown.ok) {
        cout << "ADVANCE" << endl;
        phrase->advance();
    }
    return false;
}

PhraseST::~PhraseST() {
    phrase = nullptr;
    UIRenderer::removePhrase(phrase);
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
    cout << "subtasks total : " << subtasks.size() << endl;
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
    for (auto t : subtaskTypes)
        if (t == "phrase")
            subtasks.push_back(new PhraseST(L));
}