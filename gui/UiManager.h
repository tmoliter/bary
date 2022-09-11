#ifndef UI_MANAGER_H
#define UI_MANAGER_H
#include <SDL2/SDL.h>
#include <vector>
#include <SDL2/SDL_image.h>
#include "globals.h"
#include "./Phrase.h"

using namespace std;

enum UiMode {
    Menu,
    Dialog,
    DisplayOnly,
    Hidden
};

class Phrase {
    public:
        UiMode mode;
        vector<Phrase> phrases;
};

#endif
