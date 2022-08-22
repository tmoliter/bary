#include "ThingList.h"
#include <algorithm> 

bool compareThing (Thing *thing1, Thing *thing2) {
    return (thing1->y < thing2->y);
}

void ThingList::AddThing(Thing *thing) {
    things.push_back(thing);
};

void ThingList::MeatThings(KeyPresses keysDown) {
    for (auto thing : things){
        thing->incTick();
        thing->meat(keysDown);
        thing->meat();
    }
}

void ThingList::RenderThings() {
    std::sort(things.begin(), things.end(), compareThing);
    for (auto thing : things){
        thing->render();
    }
}

void ThingList::DestroyThings() {
    for (auto thing : things){
        thing->destroy();
    }
}

void ThingList::DestroyThing() {
    // https://cplusplus.com/reference/algorithm/find/
}
