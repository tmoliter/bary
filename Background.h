#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "globals.h"

class Background {
    private:
        int width, height, screen_width, screen_height, scale;
        int *cameraX, *cameraY, &focusX, &focusY;
        SDL_Rect renderRect, sourceRect;
        SDL_Texture* texture;
        SDL_Renderer* renderer;
    public:
        Background(int *cX, int *cY, int &fX, int &fY, SDL_Renderer* renderer) : 
        cameraX(cX),
        cameraY(cY),
        focusX(fX),
        focusY(fY),
        renderer(renderer), 
        screen_width(SCREEN_WIDTH),
        screen_height(SCREEN_HEIGHT),
        scale(SCALE) {
            SDL_Surface* temp = IMG_Load("./assets/backgrounds/burg.png");
            texture = SDL_CreateTextureFromSurface(renderer, temp);
            SDL_FreeSurface(temp);
            SDL_QueryTexture(texture, NULL, NULL, &width, &height);
            sourceRect = { 0 , 0, SCREEN_WIDTH / scale, SCREEN_HEIGHT / scale };
            renderRect = { 0 , 0, SCREEN_WIDTH, SCREEN_HEIGHT };
        }
        void setPosition();
        void render();
};

#endif
