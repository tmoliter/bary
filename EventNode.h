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
    EventNode(int (*ent)(void), int (*ex)(void), int (*next)(EventNode*&), Phrase *ph);
    ~EventNode();

    Phrase *phrase;
    int (*enterAction)(void);
    int (*exitAction)(void);
    int (*nextNode)(EventNode*&);

    void loadPhrase();
    int hold(KeyPresses keysDown);
};

#endif
