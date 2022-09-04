#ifndef THING_H
#define THING_H
#include "../Input.h"
#include <iostream>
#include <map>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <fstream>
#include "../globals.h"
#include "Sprite.h"
#include <vector>
#include "../Ray.h"
#include <string>

using namespace std;

struct ThingData {
    string name;
    int x;
    int y;
};

class Thing {
    public:
        int height, width;
        Point position;
        string name;

        Thing(ThingData td);
        Thing(Point p);
        virtual ~Thing();

        void _save_name_and_write_to_map(string n);

        Point getCenter();
        
        virtual void meat() {};
        /* insane stress test */
        // virtual void meat() { int tmp = rand() % 20; y = tmp % 2 == 0 ? y + tmp : y -tmp; };

        virtual void meat(KeyPresses keysDown) {};

        inline static map<string, Thing*> things;
        inline static vector<string> thingsToDestroy;

        static int write_thing_datum(ifstream &mapData, ThingData &newTD);

        static void meatThings(KeyPresses keysDown);
        static void destroyThings();
        static void destroyThing(string n);
        static void destroyAllThings();
};

#endif
