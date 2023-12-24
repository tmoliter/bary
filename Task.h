#ifndef TASK_H
#define TASK_H
#include "things/RealThing.h"

enum class SubTaskType {
    phrase,
    wait,
    moveThing
};

struct Subtask {
    Subtask(lua_State* L);
    virtual bool meat(KeyPresses keysDown) { return true; };
};

struct WaitST : public Subtask {
    WaitST(lua_State* L);
    ~WaitST();
    Timer *timer;
    int framesToWait;
    virtual bool meat(KeyPresses keysDown);
};

struct PhraseST : public Subtask {
    PhraseST(lua_State* L);
    ~PhraseST();
    Phrase* phrase;
    virtual bool meat(KeyPresses keysDown);
};

struct Task {
    Task(string eventName, RealThing* hostThing) : eventName(eventName), hostThing(hostThing) {};
    string eventName;
    std::vector<Subtask*> subtasks;
    RealThing* hostThing;
    bool blockMeat;

    int meat(KeyPresses keysDown);

    void addSubtasks(lua_State* L);
};


#endif