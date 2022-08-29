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
};

class Thing {
    protected:
        int *cameraX, *cameraY;

    public:
        int id, x, y;

        Thing(ThingData td);
        
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
