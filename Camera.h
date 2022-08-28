#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <fstream>
#include "globals.h"

using namespace std;

class Camera {
    private:
        int *focusX, *focusY;
        SDL_Rect renderRect, sourceRect;
        SDL_Texture* bgTexture;
    public:
        Camera(SDL_Renderer* r) : 
        x(0),
        y(0),
        renderer(r),
        initialized(false) {
            c = this;
        };
        const char *path;
        bool initialized;
        int width, height;
        SDL_Renderer* renderer;
        int x, y;
        void init(int *fX, int *fY);
        void setPosition();
        void render();
        static int parse_camera(ifstream &mapData);
        inline static Camera *c;
};

#endif
