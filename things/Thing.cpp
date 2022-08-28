#include "./things/Thing.h"
#include <SDL2/SDL.h>
#include <algorithm>
#include <SDL2/SDL_image.h>

using namespace std;

Thing::Thing(ThingData td) : 
    x(td.x), 
    y(td.y) {
        id = currentID++;
        Thing::things[id] = this;
    }

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
        thing->meat(keysDown);
        thing->meat();
    }
}

void Thing::destroyThings() {
   map<int, Thing*>::iterator itr = Thing::things.begin();
   while (itr != Thing::things.end()) {
        delete itr->second;
        itr = Thing::things.erase(itr);
   }
}

void Thing::destroyThing() {
    // Destroy by ID
}