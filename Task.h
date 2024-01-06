#ifndef TASK_H
#define TASK_H
#include "things/RealThing.h"
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

struct MoveST : public Subtask {
    MoveST(lua_State* L, Host* host) : Subtask(L, host) {};
    ~MoveST();
    virtual void init();
    virtual bool meat(KeyPresses keysDown);
    RealThing* movingThing = nullptr;
    Move* move = nullptr;
    Move* prevMove = nullptr;
};

struct PortalST : public Subtask {
    PortalST(lua_State* L, Host* host) : Subtask(L, host) {};
    ~PortalST();
    virtual void init();
    virtual bool meat(KeyPresses keysDown);
    RealThing* thing = nullptr;
    Point destination;
    int newLayer;
};

struct Task {
    Task(string eventName, Host* host) : eventName(eventName), host(host) {};
    string eventName;
    std::vector<Subtask*> subtasks;
    Host* host;
    bool blocking = false; // blocking stops any older events from being executed. If we just want to pause movement, that can be done with the `pauseMoves` subtask

    int meat(KeyPresses keysDown);

    void addSubtasks(lua_State* L);

    // Instant subtasks
    void pauseMoves(lua_State* L);
};


#endif