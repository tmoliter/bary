#include "Thing.h"
#include "../components/Walk.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <iostream>

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 800
#define SCALE 1

class FieldPlayer : public Thing {
    private:
        Walk* walk;
    public:
        FieldPlayer(int &x, int &y, int *cX, int *cY, SDL_Renderer* renderer) : 
        Thing(x, y, cX, cY, renderer) {
            const int SPRITE_WIDTH = 32;
            const int SPRITE_HEIGHT = 32;
            renderRect = { 
                (SCREEN_WIDTH / 2) - ((SPRITE_WIDTH * SCALE) / 2), 
                (SCREEN_HEIGHT / 2) - (SPRITE_HEIGHT * SCALE), 
                SPRITE_WIDTH * SCALE, 
                SPRITE_HEIGHT * SCALE 
            };
            sourceRect = { 0, 0, SPRITE_WIDTH, SPRITE_HEIGHT };
            SDL_Surface* temp = IMG_Load("./assets/sheets/SDL_TestSS.png");
            texture = SDL_CreateTextureFromSurface(renderer, temp);
            SDL_FreeSurface(temp);
            walk = new Walk(x,y, sourceRect);
        };
        void init() {};

        void premeat() {};
        void meat() {};
        void meat(KeyPresses keysDown);
        void destroy();
};
