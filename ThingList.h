#ifndef THING_LIST_H
#define THING_LIST_H
#include "things/Thing.h"
#include <vector>

using namespace std;

class ThingList {
    private:
        vector <Thing*>things;
        
    public:
        ThingList() {};
        void addThing(Thing *thing);
        Thing *lastThing();
        void initThings(int *cX, int *cY, SDL_Renderer* r);
        void meatThings(KeyPresses keysDown);
        void renderThings();
        void destroyThings();
        void destroyThing();
};


#endif
