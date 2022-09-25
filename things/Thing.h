#ifndef THING_H
#define THING_H
#include "../Input.h"
#include <iostream>
#include <map>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <fstream>
#include "../globals.h"
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
    protected:
        void _save_name_and_save_in_map(string n);
    public:
        int height, width;
        Point position;
        string name;

        Thing(ThingData td);
        Thing(Point p, string name);
        Thing(Point p);
        virtual ~Thing();

        virtual void destroy();
        void rename(string newName);
        Point getCenter();
        void manuallyControl(KeyPresses keysDown);
        
        virtual void meat() {};

        virtual void meat(KeyPresses keysDown) {};

        inline static map<string, Thing*> things;
        inline static vector<string> thingsToDestroy;

        static int parse_thing_datum(ifstream &mapData, ThingData &newTD);

        static void meatThings(KeyPresses keysDown);
        static void destroyThings();
        static void destroyThing(string n);
        static void destroyAllThings();
};

#endif
