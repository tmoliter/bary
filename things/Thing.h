#ifndef THING_H
#define THING_H
#include "../Input.h"
#include <iostream>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include "../globals.h"

/* Things can be rendered */
class Thing {
    protected:
        int *cameraX, *cameraY;
    public:
        int &x, &y, tick, width, height;
        SDL_Renderer* renderer;

        SDL_Rect sourceRect, renderRect;
        SDL_Texture* texture;

        Thing(int &x, int &y, int *cX, int *cY, SDL_Renderer* renderer) : 
        x(x), 
        y(y),
        cameraX(cX),
        cameraY(cY),
        renderer(renderer),
        width(0),
        height(0),
        tick(0) {
            SDL_Surface* temp = IMG_Load("./assets/sheets/SDL_TestSS.png");
            texture = SDL_CreateTextureFromSurface(renderer, temp);
            SDL_FreeSurface(temp);
            SDL_QueryTexture(texture, NULL, NULL, &width, &height);
            renderRect = {
                -1000,
                -1000,
                width * SCALE, 
                height * SCALE 
            };
            sourceRect = { 0, 0, width, height };
        }
        virtual void init() = 0;
        
        void divideSheet(int columns, int rows) { 
            width = width / columns; 
            height = height / rows; 
            renderRect.w = width * SCALE;
            renderRect.h = height * SCALE;
            sourceRect = { 0, 0, width, height };
        }
        
        void incTick() {tick++;};
        virtual void premeat() = 0;
        virtual void meat() = 0;
        virtual void meat(KeyPresses keysDown) = 0;
        virtual void render() {
            renderRect.x = (x - *cameraX - (width / 2)) * SCALE;
            renderRect.y = (y - *cameraY - height) * SCALE;
            SDL_RenderCopy(renderer, texture, &sourceRect, &renderRect);
        };
        virtual void destroy() {SDL_DestroyTexture(texture);};
};

#endif
