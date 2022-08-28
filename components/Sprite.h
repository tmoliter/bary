#ifndef SPRITE_H
#define SPRITE_H
#include <map>
#include <../include/SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Camera.h"

using namespace std;

class Sprite {
    private:
        
    public:
        Sprite(int &x, int &y, int tI, const char *p);
        ~Sprite();
        bool active;
        int id, thingId, &x, &y, width, height, *cameraX, *cameraY;
        const char *path;
        SDL_Renderer* renderer;
        SDL_Rect sourceRect, renderRect;
        SDL_Texture* texture;

        void divideSheet(int columns, int rows);
        virtual void render();

        static int currentID;
        inline static map<int, Sprite*> sprites;
        static void renderSprites();

};

#endif
