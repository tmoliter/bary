#include "Thing.h"
#include "../components/Walk.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <iostream>


class FieldPlayer : public Thing {
    private:
        Walk* walk;
    public:
        FieldPlayer(int x, int y, int sW, int sH, int scale, SDL_Renderer* renderer) : 
        Thing(x,y,sW,sH,scale, renderer) {
            const int SPRITE_WIDTH = 32;
            const int SPRITE_HEIGHT = 32;
            renderRect = { 
                (screen_width / 2) - ((SPRITE_WIDTH * scale) / 2), 
                (screen_height / 2) - (SPRITE_HEIGHT * scale), 
                SPRITE_WIDTH * scale, 
                SPRITE_HEIGHT * scale 
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
        void render();
        void destroy();

        void getPosition(int &outerX, int &outerY);

};
