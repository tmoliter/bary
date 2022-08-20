#include <SDL2/SDL.h>
// #include <iostream>
// #include <SDL2/SDL_image.h>
// #include <thread>

class Walk {
    private:
        SDL_Rect &renderRect;
        SDL_Rect &sourceRect;
        
    public:
        Walk(SDL_Rect &renderRect, SDL_Rect &sourceRect) : renderRect(renderRect), sourceRect(sourceRect) {};
        void applySpeed(int x, int y, int speed, int cycle);
        void animate(int x, int y, int speed, int cycle);
};