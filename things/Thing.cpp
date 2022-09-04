#include "./things/Thing.h"
#include <SDL2/SDL.h>
#include <algorithm>
#include <SDL2/SDL_image.h>

using namespace std;

void Thing::_save_name_and_write_to_map(string n) {
    if(n == "") {
        n = "AnonymousThing";
    }
    name = n;
    int i = 2;
    while(Thing::things.count(name)) {
        name = n + to_string(i);
        i++;
    }
    Thing::things[name] = this; 
}

Thing::Thing(ThingData td) : 
    position(td.x, td.y),
    height(0), 
    width(0) {
        _save_name_and_write_to_map(td.name);
    }

Thing::Thing(Point p) : 
    position(p.x,p.y),
    height(0), 
    width(0) {
        _save_name_and_write_to_map("");

}

Thing::~Thing() {
    things.erase(name);
}

Point Thing::getCenter() {
    return Point(position.x + (width / 2) , position.y + (height / 2));
}

// STATIC

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
   map<string, Thing*>::iterator itr = Thing::things.begin();
   while (itr != Thing::things.end()) {
        delete itr->second;
        itr = Thing::things.erase(itr);
   }
}

void Thing::destroyThing(string n) {
    thingsToDestroy.push_back(n);
}