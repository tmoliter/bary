#ifndef THING_H
#define THING_H
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include "../Input.h"
#include "../globals.h"
#include "../Ray.h"

using namespace std;

struct ThingData {
    string name;
    int x;
    int y;
};

class Thing {
    protected:
        virtual void _save_name_and_save_in_map(string n);
    public:
        Point position;
        Bounds bounds;
        string name;

        Thing(ThingData td);
        Thing(Point p, string name);
        Thing(Point p);
        virtual ~Thing();

        // Note for Door, etc.:
        // Have a vector of Thing pointers called dependencies
        // When we create a Door, we pass in its parent's name, if any, 
        // and call Thing::things[parentName]->addDependency().
        // When a Thing is deleted or moved in any way, all of its dependencies must follow suit.
        // We might also store this relationship in a static graph of pointers,
        // in addition to a list of Point &references or something if we think we want to optimize
        // Maybe something like map<Thing*, Point&>

        virtual void destroy();
        string rename(string newName);
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

        static vector<Thing*> findThingsByPoint(Point p);
};

#endif
