#include "./things/Thing.h"
#include <SDL2/SDL.h>
#include <algorithm>
#include <SDL2/SDL_image.h>

using namespace std;

Thing::Thing(ThingData td) : 
    position(td.x, td.y),
    height(0), 
    width(0),
    name(td.name) {
        // We should use the name as an id and guarantee uniqueness somehow
        id = currentID++;
        Thing::things[id] = this;
    }

Thing::Thing(int x, int y) : 
    position(x,y),
    height(0), 
    width(0), 
    name("name") {
        cout <<" WTF: " << height << endl;
        // We should use the name as an id and guarantee uniqueness somehow
        id = currentID++;
        Thing::things[id] = this;
}


Thing::~Thing() {
    things.erase(id);
}

void Thing::getCenter(int &cX, int &cY) {
    // cout << "HEIGHT: " << height << endl;
    // cout << "WIDTH: " << width << endl;
    cX = position.x + (width / 2);
    cY = position.y + (height / 2);
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
                    newTD.name = value;
                    value.clear();
                    break;
                case (2):
                    index++;
                    newTD.x = std::stoi(value);
                    value.clear();
                    break;
                case (3):
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
    for (auto killMe : thingsToDestroy) {
        Thing* thing = Thing::things[killMe];
        delete thing;
    }
    thingsToDestroy.clear();
}

void Thing::destroyAllThings() {
   map<int, Thing*>::iterator itr = Thing::things.begin();
   while (itr != Thing::things.end()) {
        delete itr->second;
        itr = Thing::things.erase(itr);
   }
}

void Thing::destroyThing(int id) {
    thingsToDestroy.push_back(id);
}