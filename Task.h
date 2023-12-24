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
    ~Subtask();
    virtual bool meat(KeyPresses keysDown);
    Timer *timer;
    int framesToWait;
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