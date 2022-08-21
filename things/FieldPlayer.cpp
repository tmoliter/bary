#include "FieldPlayer.h"
#include <../include/SDL2/SDL_image.h>
#include <iostream>

using namespace std;

void FieldPlayer::init() {
    const int SPRITE_WIDTH = 32;
    const int SPRITE_HEIGHT = 32;
    renderRect = { 50, 50, SPRITE_WIDTH * 2, SPRITE_HEIGHT * 2 };
    sourceRect = { 0, 0, SPRITE_WIDTH, SPRITE_HEIGHT };
    SDL_Surface* temp = IMG_Load("./assets/sheets/SDL_TestSS.png");
    texture = SDL_CreateTextureFromSurface(renderer, temp);
    SDL_FreeSurface(temp);
    walk = new Walk(renderRect, sourceRect);
};

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

    walk->move(xV,yV,tick);
    walk->animate(xV,yV,tick);
};

void FieldPlayer::render() {
    SDL_RenderCopy(renderer, texture, &sourceRect, &renderRect);
};

void FieldPlayer::destroy() { Thing::destroy(); delete walk; delete this;};

void FieldPlayer::getPosition(int &outerX, int &outerY) {
    outerX = x;
    outerY = y;
}