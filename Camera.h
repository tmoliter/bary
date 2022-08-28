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
        Camera() : 
        x(0),
        y(0),
        initialized(false) {
            window = SDL_CreateWindow(
                        "Timmy's Big Test",
                        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                        SCREEN_WIDTH, SCREEN_HEIGHT,
                        SDL_WINDOW_SHOWN
                        );

            renderer = SDL_CreateRenderer(window, -1, 0);
            c = this;
        };
        const char *path;
        bool initialized;
        int width, height;
        SDL_Renderer* renderer;
        SDL_Window* window;
        int x, y;
        void init(int *fX, int *fY);
        void setPosition();
        void render();
        static int parse_camera(ifstream &mapData);
        inline static Camera *c;
};

#endif
