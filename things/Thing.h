#include <../include/SDL2/SDL.h>
#include "../input.h"

/* Things can be rendered */
class Thing {
    public:
        int x, y, tick;
        SDL_Renderer* renderer;

        SDL_Rect sourceRect, renderRect;
        SDL_Texture* texture;

        Thing(int x, int y, SDL_Renderer* renderer) : x(x), y(y), renderer(renderer), tick(0) {}
        virtual void init() = 0;

        void incTick() {tick++;};
        virtual void premeat() = 0;
        virtual void meat() = 0;
        virtual void meat(KeyPresses keysDown) = 0;
        virtual void render() = 0;
        
        virtual void destroy() {SDL_DestroyTexture(texture);};
};