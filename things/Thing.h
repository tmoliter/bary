#ifndef THING_H
#define THING_H
#include "../Input.h"
#include <iostream>
#include <map>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <fstream>
#include "../globals.h"
#include "../Camera.h"
#include "Sprite.h"

using namespace std;

struct ThingData {
    int x;
    int y;
    const char *path;
};

class Thing {
    protected:
        int *cameraX, *cameraY;

    public:
        const char *path;
        int id, x, y, tick, width, height;

        Sprite *sprite;

        Thing(ThingData td);
        ~Thing();
        
        void incTick();
        
        virtual void meat() {};
        /* insane stress test */
        // virtual void meat() { int tmp = rand() % 20; y = tmp % 2 == 0 ? y + tmp : y -tmp; };

        virtual void meat(KeyPresses keysDown) {};

        static int write_thing_datum(ifstream &mapData, ThingData &newTD);
        static int currentID;
        inline static map<int, Thing*> things;

        static void meatThings(KeyPresses keysDown);
        static void destroyThings();
        static void destroyThing();
};

#endif
