#include "./things/Thing.h"
#include <SDL2/SDL.h>
#include <algorithm>
#include <SDL2/SDL_image.h>

using namespace std;

void Thing::_save_name_and_save_in_map(string n) {
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
        _save_name_and_save_in_map(td.name);
    }

Thing::Thing(Point p) : 
    position(p.x,p.y),
    height(0), 
    width(0) {
        _save_name_and_save_in_map("AnonymousThing");
}

Thing::~Thing() {
    things.erase(name);
}

void Thing::destroy() {
    Thing::destroyThing(name);
}

Point Thing::getCenter() {
    return Point(position.x + (width / 2) , position.y + (height / 2));
}

// STATIC

int Thing::parse_thing_datum(ifstream &mapData, ThingData &newTD) {
    mapData.get();
    utils::parse_strings(vector <string*> { &newTD.name }, mapData);
    utils::parse_ints(vector <int*> { &newTD.x, &newTD.y }, mapData);
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