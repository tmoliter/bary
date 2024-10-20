#ifndef TASK_H
#define TASK_H
#include "Scene.h"
#include <set>

enum class SubTaskType {
    phrase,
    wait,
    moveThing
};

struct Subtask {
    Subtask(lua_State* L, Host* host);
    virtual ~Subtask();
    lua_State* L;
    Host* host;
    virtual bool pushArgs() { return false; };
    virtual void init() {};
    virtual bool meat(KeyPresses keysDown);
    Timer *timer;
    int framesToWait;
};

struct PhraseST : public Subtask {
    PhraseST(lua_State* L, Host* host) : Subtask(L, host) {};
    ~PhraseST();
    virtual void init();
    virtual bool meat(KeyPresses keysDown);
    Phrase* phrase = nullptr;
};

struct MenuST : public Subtask {
    MenuST(lua_State* L, Host* host) : Subtask(L, host) {};
    virtual void init();
    virtual bool meat(KeyPresses keysDown);
    virtual bool pushArgs();
    string selection;
    MenuDisplay* menu = nullptr;
};

struct MoveST : public Subtask {
    MoveST(lua_State* L, Host* host) : Subtask(L, host) {};
    ~MoveST();
    virtual void init();
    virtual bool meat(KeyPresses keysDown);
    RealThing* movingThing = nullptr;
    Move* move = nullptr;
    Move* prevMove = nullptr;

    // TESTING
    virtual bool pushArgs() { 
        luaUtils::PushIntToTable(L, "someBullshit", 69);
        return true; 
    };
};

struct PortalST : public Subtask {
    PortalST(lua_State* L, Host* host) : Subtask(L, host) {};
    ~PortalST();
    virtual void init();
    virtual bool meat(KeyPresses keysDown);
    RealThing* thing = nullptr;
    Point destination;
    int newLayer;
    Scene* newScene;
};

// A task is a group of "subtasks", which can be fired from an event in lua.
// An event is a lua function that gets called as a coroutine and can fire off
// Tasks in between yield statements
struct Task {
    Task(string eventName, Host* host, int argKey) : eventName(eventName), host(host), argKey(argKey) {};
    string eventName;
    std::vector<Subtask*> subtasks;
    Host* host;

    // I didn't document this at all when I wrote it. Looking back, I'm
    // retty sure this stores a table of args for continuity throughout events
    int argKey = LUA_NOREF;

    bool blocking = false; // blocking stops any older events from being executed. If we just want to pause movement, that can be done with the `pauseMoves` subtask
    int meat(KeyPresses keysDown);

    void killEvent(); // removes all subtasks in C++ and removes task from Lua activeEvents table

    void addSubtasks(lua_State* L);

    // Instant subtasks
    void pauseMoves(lua_State* L);
};


#endif
