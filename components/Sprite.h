#ifndef SPRITE_H
#define SPRITE_H
#include <map>
#include <string>
#include <../include/SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Camera.h"

using namespace std;

struct SpriteData {
    int layer = 0;
    int renderOffset = 0;
    int xOffset = 0;
    int yOffset = 0;
    int sourceX = 0;
    int sourceY = 0;
    int width = 0;
    int height = 0;
    SDL_Texture *texture;
};

class Sprite {
    public:
        Sprite(int &x, int &y, int &tI, SpriteData sd);
        ~Sprite();
        bool active;
        int id, &thingId, &x, &y, layer, renderOffset, yOffset, xOffset, width, height, *cameraX, *cameraY;
        SDL_Renderer* renderer;
        SDL_Rect sourceRect, renderRect;
        SDL_Texture* texture;

        void divideSheet(int columns, int rows);
        virtual void render();

        static int currentID;
        inline static map<int, Sprite*> sprites;
        inline static map<string, SDL_Texture*> textures;
        static void renderSprites();

        static int write_sprite_datum(ifstream &mapData, SpriteData &newSD);
};

#endif
