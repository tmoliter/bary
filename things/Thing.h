#ifndef THING_H
#define THING_H
#include "../Input.h"
#include <iostream>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <fstream>
#include "../globals.h"

using namespace std;

struct ThingData {
    int id;
    int x;
    int y;
    const char *path;
};

/* Things can be rendered */
class Thing {
    protected:
        int *cameraX, *cameraY;

    public:
        bool initialized;
        const char *path;
        int id, x, y, tick, width, height;
        SDL_Renderer* renderer;

        SDL_Rect sourceRect, renderRect;
        SDL_Texture* texture;

        Thing(ThingData td) : 
        id(td.id),
        x(td.x), 
        y(td.y),
        path(td.path),
        width(0),
        height(0),
        tick(0),
        initialized(false) {}

        virtual void init (int *cX, int *cY, SDL_Renderer* renderer);
        
        void divideSheet(int columns, int rows);
        
        void incTick();
        
        virtual void meat() {};
        /* insane stress test */
        // virtual void meat() { int tmp = rand() % 20; y = tmp % 2 == 0 ? y + tmp : y -tmp; };

        virtual void meat(KeyPresses keysDown) {};
        virtual void render();
        virtual void destroy();

        static int write_thing_datum(ifstream &mapData, ThingData &newTD);
};

#endif
