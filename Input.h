#ifndef INPUT_H
#define INPUT_H
#include <SDL2/SDL_image.h>
#include <iostream>
#include "globals.h"

struct KeyPresses {
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
    bool nav_up = false;
    bool nav_down = false;
    bool nav_left = false;
    bool nav_right = false;
    bool ok = false;
    bool cancel = false;
    bool menu1 = false;
    bool menu2 = false;
    bool start = false;
    bool quit = false;
    char textInput = false;
    bool del = false;
    /* DEBUG */
    bool debug_plus = false;
    bool debug_minus = false;
    bool debug_up = false;
    bool debug_down = false;
    bool debug_left = false;
    bool debug_right = false;
    bool debug_1 = false;
    bool debug_2 = false;
    bool debug_3 = false;
    bool debug_4 = false;
    bool debug_5 = false;
    bool debug_6 = false;
    bool debug_7 = false;
    bool debug_8 = false;
    bool debug_9 = false;
    bool debug_0 = false;
    /* END DEBUG */
};

class Input {
    private:
        KeyPresses keysDown;
    public:
        KeyPresses& getInput();
        Input() {KeyPresses keysDown;}
};

#endif
