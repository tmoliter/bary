#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H
#include "Task.h"
using namespace luaUtils;

struct GameController : public Host {
    GameController(lua_State *L);

    vector<Task*> activeTasks;
    map<pair<Host*, string> ,int> eventArgKeys;

    map<string, RealThing*> mockSceneThings;
    RealThing* gameManager;  // Invisible thing used to register events that aren't attached to an actual thing

    void meat(KeyPresses keysDown);
    bool meatEvent(KeyPresses keysDown);

    static int _newTask(lua_State *L);
    static int _killAllTasksForThing(lua_State *L);

    inline static GameController *controller;
};

#endif
