#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <fstream>
#include <string>
#include "globals.h"
#include "./things/Thing.h"

using namespace std;

class Camera {
    private:
        SDL_Rect renderRect, sourceRect;
        SDL_Texture* bgTexture;
    public:
        Thing* focus;
        const char *path;
        bool initialized;
        int width, height;
        SDL_Renderer* renderer;

        Camera(SDL_Renderer* r) : 
        renderer(r),
        initialized(false) {
            c = this;
        };
        void init(Thing *f);
        void setPosition();
        void render();

        inline static Camera *c;

        static int parse_camera(ifstream &mapData);
        static void panTo(string thingName);
        static string getFocusName();
};

#endif
