#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "globals.h"

class Background {
    private:
        int width, height;
        int *focusX, *focusY;
        SDL_Rect renderRect, sourceRect;
        SDL_Texture* texture;
        SDL_Renderer* renderer;
    public:
        int cameraX, cameraY;
        Background(int *fX, int *fY, SDL_Renderer* renderer) : 
        cameraX(0),
        cameraY(0),
        focusX(fX),
        focusY(fY),
        renderer(renderer) {
            SDL_Surface* temp = IMG_Load("./assets/backgrounds/burg.png");
            texture = SDL_CreateTextureFromSurface(renderer, temp);
            SDL_FreeSurface(temp);
            SDL_QueryTexture(texture, NULL, NULL, &width, &height);
            sourceRect = { 0 , 0, SCREEN_WIDTH / SCALE, SCREEN_HEIGHT / SCALE };
            renderRect = { 0 , 0, SCREEN_WIDTH, SCREEN_HEIGHT };
        }
        void setPosition();
        void setFocus(int *x, int *y);
        void render();
};

#endif
