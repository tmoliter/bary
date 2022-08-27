#include "ThingList.h"
#include <algorithm> 
#include <iostream>

bool compareThing (Thing *thing1, Thing *thing2) {
    return (thing1->y < thing2->y);
}

void ThingList::addThing(Thing *thing) {
    things.push_back(thing);
}

Thing *ThingList::lastThing() {
    return things[things.size() - 1];
};

void ThingList::initThings(int *cX, int *cY, SDL_Renderer* r) {
    for (auto thing : things){
        thing->init(cX,cY,r);
    }
}

void ThingList::meatThings(KeyPresses keysDown) {
    for (auto thing : things){
        thing->incTick();
        thing->meat(keysDown);
        thing->meat();
    }
}
void ThingList::renderThings() {
    std::sort(things.begin(), things.end(), compareThing);
    for (auto thing : things){
        thing->render();
    }
}

void ThingList::destroyThings() {
    for (auto thing : things){
        thing->destroy();
    }
}

void ThingList::destroyThing() {
    // https://cplusplus.com/reference/algorithm/find/
}
