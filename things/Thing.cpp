#include "./things/Thing.h"
#include <SDL2/SDL.h>
#include <algorithm>
#include <SDL2/SDL_image.h>

using namespace std;

Thing::Thing(ThingData td) : 
    x(td.x), 
    y(td.y),
    path(td.path),
    width(0),
    height(0),
    tick(0) {
        id = currentID++;
        Thing::things[id] = this;
        sprite = new Sprite(x,y,id,path);
    }

Thing::~Thing() {
    delete sprite;
}

void Thing::incTick() {tick++;};

void Thing::destroyInLoop(map<int, Thing*>::iterator &itr) {
    itr = Thing::things.erase(itr);
    delete this;
};

// STATIC

int Thing::currentID = 0;

int Thing::write_thing_datum(ifstream &mapData, ThingData &newTD) {
    int index = 0;
    string value = "";
    char current;
    while(mapData.get(current)) {
        if (current == ',') {
            switch(index) {
                case (0):
                    index++;
                    break;
                case (1):
                    index++;
                    newTD.x = std::stoi(value);
                    value.clear();
                    break;
                case (2):
                    index++;
                    newTD.y = std::stoi(value);
                    value.clear();
                    break;
                case (3):
                    newTD.path = strdup(value.c_str());
                    value.clear();
                    return 1;
                default:
                    return 0;
            }
            continue;
        }
        value.push_back(current);
    }
    return 1;
}

void Thing::meatThings(KeyPresses keysDown) {
    for (auto const& [id, thing] : Thing::things){
        thing->incTick();
        thing->meat(keysDown);
        thing->meat();
    }
}

void Thing::destroyThings() {
   map<int, Thing*>::iterator itr = Thing::things.begin();
   while (itr != Thing::things.end()) {
        itr->second->destroyInLoop(itr);
   }
}

void Thing::destroyThing() {
    // https://cplusplus.com/reference/algorithm/find/
}