#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <fstream>
#include <string>
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
        Point focalPoint;
        FxStatus fadeStatus, warpStatus;
        int 
            bgWidth, bgHeight, 
            scaledScreenWidth, scaledScreenHeight,  
            fadeMultiplier, warpMultiplier;
        string bgTextureName;
        bool initialized;

        Camera() : 
        focalPoint(Point(0,0)),
        initialized(false),
        warpStatus(FxStatus::unapplied),
        fadeStatus(FxStatus::applied),
        fadeMultiplier(1),
        warpMultiplier(1),
        scaledScreenWidth(settings.SCREEN_WIDTH / settings.SCALE),
        scaledScreenHeight(settings.SCREEN_HEIGHT / settings.SCALE) {
            c = this;
        };
        void init();

        void setPosition();
        Point getSourceRectCoords();
        void renderBackground();
        void renderAfterEffects();


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
};

#endif
