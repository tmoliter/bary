#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "globals.h"

class Camera {
    private:
        int *focusX, *focusY;
        SDL_Rect renderRect, sourceRect;
        SDL_Texture* bgTexture;
    public:
        const char *path;
        bool initialized;
        int width, height;
        SDL_Renderer* renderer;
        int x, y;
        Camera(SDL_Renderer* r, const char *p) : 
        x(0),
        y(0),
        renderer(r),
        path(p),
        initialized(false) {};
        void init(int *fX, int *fY);
        void setPosition();
        void setFocus(int *fX, int *fY);
        void render();
};

#endif
