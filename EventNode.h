#ifndef EVENT_NODE_H
#define EVENT_NODE_H
#include <string>
#include <queue>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "globals.h"
#include "./gui/Phrase.h"
#include "Input.h"

using namespace std;

struct EventNode {
    EventNode() {};
    ~EventNode() {};

    Phrase *phrase;
    int (*enterAction)(void);
    int (*exitAction)(void);
    EventNode *(*nextNode)(void);

    // Destructor should take care of all phrases and action functions, unless action is a reusable

    int hold(KeyPresses keysDown) { return 1; };
};

#endif
