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
#include <vector>

using namespace std;

struct ThingData {
    string name;
    int x;
    int y;
};

class Thing {
    protected:
        int *cameraX, *cameraY;

    public:
        int id, x, y, c2x, c2y;
        string name;

        Thing(ThingData td);
        Thing(int x, int y);
        virtual ~Thing();
        
        virtual void meat() {};
        /* insane stress test */
        // virtual void meat() { int tmp = rand() % 20; y = tmp % 2 == 0 ? y + tmp : y -tmp; };

        virtual void meat(KeyPresses keysDown) {};

        static int write_thing_datum(ifstream &mapData, ThingData &newTD);
        static int currentID;
        inline static map<int, Thing*> things;
        inline static vector<int> thingsToDestroy;

        static void meatThings(KeyPresses keysDown);
        static void destroyThings();
        static void destroyThing(int id);
        static void destroyAllThings();
};

#endif
