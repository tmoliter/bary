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
        void AddThing(Thing *thing);
        void MeatThings(KeyPresses keysDown);
        void RenderThings();
        void DestroyThings();
};


#endif
