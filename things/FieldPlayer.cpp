#include "FieldPlayer.h"
#include <../include/SDL2/SDL_image.h>
#include <iostream>

using namespace std;

void FieldPlayer::meat(KeyPresses keysDown) {
    int xV = 0, yV = 0;
    if (keysDown.left)
        xV -= 1;
    if (keysDown.right)
        xV += 1;
    if (keysDown.up)
        yV -= 1;
    if (keysDown.down)
        yV += 1;

    if (keysDown.ok)
        walk->changeSpeed(false);
    if (keysDown.cancel)
        walk->changeSpeed(true);

    walk->move(xV,yV,x,y,tick);
    walk->animate(xV,yV,tick);
};

void FieldPlayer::destroy() { Thing::destroy(); delete walk; delete this;};
