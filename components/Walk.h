#include <../include/SDL2/SDL.h>

class Walk {
    private:
        SDL_Rect &renderRect;
        SDL_Rect &sourceRect;
        int speed;
        
    public:
        Walk(SDL_Rect &renderRect, SDL_Rect &sourceRect) : renderRect(renderRect), sourceRect(sourceRect), speed(2) {};
        void move(int x, int y, int tick);
        void animate(int x, int y, int tick);
        void changeSpeed(bool decrease);
};