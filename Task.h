#ifndef TASK_H
#define TASK_H
#include "things/RealThing.h"

enum class SubTaskType {
    phrase,
    wait,
    moveThing
};

struct Subtask {
    Subtask(lua_State* L, map<string, RealThing*>& things);
    virtual ~Subtask();
    lua_State* L;
    map<string, RealThing*>& things;
    virtual void init() {};
    virtual bool meat(KeyPresses keysDown);
    Timer *timer;
    int framesToWait;
};

struct PhraseST : public Subtask {
    PhraseST(lua_State* L, map<string, RealThing*>& things) : Subtask(L, things) {};
    ~PhraseST();
    virtual void init();
    virtual bool meat(KeyPresses keysDown);
    Phrase* phrase = nullptr;
};

struct MoveST : public Subtask {
    MoveST(lua_State* L, map<string, RealThing*>& things) : Subtask(L, things) {};
    ~MoveST();
    virtual void init();
    virtual bool meat(KeyPresses keysDown);
    RealThing* movingThing = nullptr;
    Move* move = nullptr;
    Move* prevMove = nullptr;
};


struct Task {
    Task(string eventName, RealThing* hostThing, map<string, RealThing*>& things) : eventName(eventName), hostThing(hostThing), things(things) {};
    string eventName;
    std::vector<Subtask*> subtasks;
    RealThing* hostThing;
    map<string, RealThing*>& things;
    bool blockMeat;

    int meat(KeyPresses keysDown);

    void addSubtasks(lua_State* L);
};


#endif