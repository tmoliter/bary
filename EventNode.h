#ifndef EVENT_NODE_H
#define EVENT_NODE_H
#include <string>
#include <queue>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "globals.h"
#include "./gui/Phrase.h"

using namespace std;

struct EventNode {
    Phrase *phrase;
    void (*enterAction)(void);
    void (*exitAction)(void);
    EventNode (*nextNode)(void);

    void execute();
};

#endif
