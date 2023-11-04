#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <fstream>
#include <string>
#include "globals.h"
#include "utils.h"
#include "./components/Sprite.h"

using namespace std;

enum class FxStatus {
    applying,
    unapplying,
    applied,
    unapplied
};

class Camera {
    private:
        SDL_Rect renderRect, sourceRect;
        SDL_Texture* bgTexture;
        int fadeStart, warpStart;
    public:
        SDL_Renderer* renderer;
        Point focalPoint;
        FxStatus fadeStatus, warpStatus;
        int 
            bgWidth, bgHeight, 
            scaledScreenWidth, scaledScreenHeight,  
            fadeMultiplier, warpMultiplier;
        const char *path;
        bool initialized;

        Camera(SDL_Renderer* r) : 
        renderer(r),
        focalPoint(Point(0,0)),
        initialized(false),
        warpStatus(FxStatus::unapplied),
        fadeStatus(FxStatus::applied),
        fadeMultiplier(2),
        warpMultiplier(1),
        scaledScreenWidth(SCREEN_WIDTH / SCALE),
        scaledScreenHeight(SCREEN_HEIGHT / SCALE) {
            c = this;
        };
        void init();

        void setPosition();
        Point getSourceRectCoords();
        void render();


        void handleFade();
        void setOverlay();

        void setWarpLevel();

        inline static Camera *c;

        static int fadeIn(int m);
        static int fadeOut(int m);
        static void warpIn(int m);
        static void warpOut(int m);

        static Point getPos();

        static Point worldToScreen(Point p);
        static Ray worldToScreen(Ray r);

        static int parse_camera(ifstream &mapData);
};

#endif
