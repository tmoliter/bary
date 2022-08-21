#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

class Background {
    private:
        int screen_width, screen_height, width, height, scale;
        SDL_Rect renderRect, sourceRect;
        SDL_Texture* texture;
        SDL_Renderer* renderer;
    public:
        Background(int w, int h, int scale, SDL_Renderer* renderer) : 
        renderer(renderer),
        screen_width(w), 
        screen_height(h),
        scale(scale) {
            SDL_Surface* temp = IMG_Load("./assets/backgrounds/burg.png");
            texture = SDL_CreateTextureFromSurface(renderer, temp);
            SDL_FreeSurface(temp);
            sourceRect = { 0 , 0, screen_width, screen_height };
            renderRect = { 0 , 0, screen_width * scale, screen_height * scale };
        }
        void render(int &centerX, int &centerY);
};
