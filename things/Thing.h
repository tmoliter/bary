#include <../include/SDL2/SDL.h>
#include "../Input.h"

/* Things can be rendered */
class Thing {
    protected:
        int *cameraX, *cameraY;
    public:
        int &x, &y, tick;
        SDL_Renderer* renderer;

        SDL_Rect sourceRect, renderRect;
        SDL_Texture* texture;

        Thing(int &x, int &y, int *cX, int *cY, SDL_Renderer* renderer) : 
        x(x), 
        y(y),
        cameraX(cX),
        cameraY(cY),
        renderer(renderer), 
        tick(0) {}
        virtual void init() = 0;

        void incTick() {tick++;};
        virtual void premeat() = 0;
        virtual void meat() = 0;
        virtual void meat(KeyPresses keysDown) = 0;
        virtual void render() {
            SDL_RenderCopy(renderer, texture, &sourceRect, &renderRect);
        };
        virtual void destroy() {SDL_DestroyTexture(texture);};
};