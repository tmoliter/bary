#include <../include/SDL2/SDL.h>

class Background {
    private:
        SDL_Renderer* renderer;
    public:
        Background(SDL_Renderer* renderer) : renderer(renderer) {}
        void render();
        void getPosition(int &outerX, int &outerY);
};
