#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H
#include "Task.h"
using namespace luaUtils;

struct GameController : public Host {
    GameController(lua_State *L);

    vector<Task*> activeTasks;
    map<pair<Host*, string> ,int> eventArgKeys;

    struct PendingSceneChange {
        bool active = false;
        Scene* newScene = nullptr;
        RealThing* thing = nullptr;  // thing to carry into the new scene (e.g. the player)
        Point destination;
        int newLayer = 0;
    } pendingSceneChange;

    void requestSceneChange(Scene* newScene, RealThing* thing, Point destination, int newLayer);
    void performPendingSceneChange();
    void killAllTasks();
    void killEvent(Host* host, string eventName);

    void meat(KeyPresses keysDown);
    bool meatEvent(KeyPresses keysDown);

    static int _newTask(lua_State *L);
    static int _killAllTasksForThing(lua_State *L);

    inline static GameController *controller;
};

#endif
