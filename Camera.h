#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <fstream>
#include <string>
#include "globals.h"
#include "utils.h"
#include "./things/Thing.h"
#include "./things/GhostFocus.h"
#include "./components/Sprite.h"

using namespace std;

enum class FxStatus {
    applying,
    unapplying,
    applied,
    unapplied
};

enum class FocusMode {
    center,
    point
};

class Camera {
    private:
        SDL_Rect renderRect, sourceRect;
        SDL_Texture* bgTexture;
        int fadeStart, warpStart;
    public:
        Thing* focus;
        SDL_Renderer* renderer;
        FxStatus fadeStatus, warpStatus;
        FocusMode focusMode;
        int 
            bgWidth, bgHeight, 
            scaledScreenWidth, scaledScreenHeight,  
            fadeMultiplier, warpMultiplier;
        const char *path;
        bool initialized;

        Camera(SDL_Renderer* r) : 
        renderer(r),
        initialized(false),
        warpStatus(FxStatus::unapplied),
        fadeStatus(FxStatus::applied),
        fadeMultiplier(2),
        warpMultiplier(1),
        scaledScreenWidth(SCREEN_WIDTH / SCALE),
        scaledScreenHeight(SCREEN_HEIGHT / SCALE) {
            c = this;
        };
        void init(Thing *f);

        void setPosition();
        void render();


        void handleFade();
        void setOverlay();

        void setWarpLevel();

        inline static Camera *c;

        static void panTo(string thingName);
        static int fadeIn(int m);
        static int fadeOut(int m);
        static void warpIn(int m);
        static void warpOut(int m);

        static string getFocusName();
        static Point getPos();
        static void setFocusMode(FocusMode newMode);

        static Point worldToScreen(Point p);
        static Ray worldToScreen(Ray r);

        static int parse_camera(ifstream &mapData);
};

#endif
