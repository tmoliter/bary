#ifndef EVENT_H
#define EVENT_H
#include <string>
#include <queue>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "globals.h"
#include "./EventNode.h"
#include "Input.h"

using namespace std;

enum class EventStage {
    pending,
    enterNode,
    holdNode,
    exitNode,
    terminateEvent,
};

struct Event {
    EventNode* current;
    int references;

    Event();
    ~Event();

    EventNode* firstNode;
    vector<EventNode*> nodes;
    virtual EventNode* addNode(EventNode* node);
    void addNodes(vector<EventNode*> newNodes);
    EventNode* addChild(EventNode* prev, EventNode* newNode);

    void begin();

    void executeEnter();
    void holdNode(KeyPresses keysDown);
    void endNode();

    void terminate();

    void setBeginGameState();
    void setEndGameState();

    EventStage stage;

    static void meat(KeyPresses keysDown);
    inline static vector<Event*> activeEvents;
};

#endif
