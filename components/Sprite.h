#ifndef SPRITE_H
#define SPRITE_H
#include <map>
#include <../include/SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Camera.h"

using namespace std;

struct SpriteData {
    int layer;
    int xOffset;
    int yOffset;
    const char* path;
};

class Sprite {
    public:
        Sprite(int &x, int &y, int &tI, SpriteData sd);
        ~Sprite();
        bool active;
        int id, &thingId, &x, &y, layer, yOffset, xOffset, width, height, *cameraX, *cameraY;
        const char *path;
        SDL_Renderer* renderer;
        SDL_Rect sourceRect, renderRect;
        SDL_Texture* texture;

        void divideSheet(int columns, int rows);
        virtual void render();

        static int currentID;
        inline static map<int, Sprite*> sprites;
        static void renderSprites();

        static int write_sprite_datum(ifstream &mapData, SpriteData &newSD);
};

#endif