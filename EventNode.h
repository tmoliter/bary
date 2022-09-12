#ifndef EVENT_NODE_H
#define EVENT_NODE_H
#include <string>
#include <queue>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "globals.h"
#include "./gui/Phrase.h"
#include "./gui/UIRenderer.h"
#include "Input.h"

using namespace std;

struct EventNode {
    EventNode(EventNode **nn = nullptr, Phrase *ph = nullptr, int (*ent)(void) = nullptr, int (*ex)(void) = nullptr, int (*nextCB)(EventNode*&) = nullptr);
    ~EventNode();

    EventNode **nextNode;
    Phrase *phrase;
    int (*enterAction)(void);
    int (*exitAction)(void);
    int (*nextNodeCB)(EventNode*&);

    int getNextNode(EventNode *&node);
    void loadPhrase();
    int hold(KeyPresses keysDown);
};

#endif
