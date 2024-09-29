#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H
#include "Task.h"
using namespace luaUtils;

struct GameController : public Host {
    GameController(lua_State *L);

    vector<Task*> activeTasks;
    map<pair<Host*, string> ,int> eventArgKeys;

    void meat(KeyPresses keysDown);
    bool meatEvent(KeyPresses keysDown);

    static int _newTask(lua_State *L);

    inline static GameController *controller;
};

#endif
