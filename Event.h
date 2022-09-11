#ifndef EVENT_H
#define EVENT_H
#include <string>
#include <queue>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "globals.h"
#include "./EventNode.h"
#include "Input.h"

using namespace std;

enum EventStage {
    enterNode,
    holdNode,
    exitNode,
    terminateEvent,
};

struct Event {
    EventNode* current;

    Event();
    ~Event();

    // No need to destroy nodes until Event destructor is called
    vector<EventNode*> nodes;
    void addNode(EventNode* node);


    void begin();

    void executeEnter();
    void holdNode(KeyPresses keysDown);
    void endNode();

    void terminate();

    EventStage stage;
    
    static void meat(KeyPresses keysDown);
    inline static vector<Event*> activeEvents;

};

#endif
