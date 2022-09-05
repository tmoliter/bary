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

enum FxStatus {
    applying,
    unapplying,
    applied,
    unapplied
};

class Camera {
    private:
        SDL_Rect renderRect, sourceRect;
        SDL_Texture* bgTexture;
        int fadeStart;
    public:
        Thing* focus;
        SDL_Renderer* renderer;
        FxStatus fadeStatus;
        int width, height, fadeMultiplier;
        const char *path;
        bool initialized;

        Camera(SDL_Renderer* r) : 
        renderer(r),
        initialized(false),
        fadeStatus(applied),
        fadeMultiplier(2) {
            c = this;
        };
        void init(Thing *f);

        void setPosition();
        void render();

        void handleFade();
        void setOverlay();

        inline static Camera *c;

        static int parse_camera(ifstream &mapData);
        static void panTo(string thingName);
        static void fadeIn(int m);
        static void fadeOut(int m);
        static string getFocusName();
};

#endif
