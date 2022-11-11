#include "things/Thing.h"

using namespace std;

void Thing::_save_name_and_save_in_map(string n) {
    int i;
    for (i = 0; i < n.length(); i++)
        if (isdigit(n[i]))
            break;
    string baseName = n.substr(0, i);
    i = 1;
    name = baseName;
    while(Thing::things.count(name)) {
        name = baseName + to_string(i);
        i++;
    }
    Thing::things[name] = this;
}

Thing::Thing(ThingData td) : 
    position(td.x, td.y) {
    _save_name_and_save_in_map(td.name);
}

Thing::Thing(Point p, string name) : 
    position(p.x,p.y),
    name(name) {
    _save_name_and_save_in_map(name);
}


Thing::Thing(Point p) : 
    position(p.x,p.y) {
    _save_name_and_save_in_map("AnonymousThing");
}

Thing::Thing(Thing &oldThing) : position(oldThing.position), bounds(oldThing.bounds) {
    string n = oldThing.name;
    _save_name_and_save_in_map(n);
};

Thing::~Thing() {
    things.erase(name);
    for (auto t : subThings)
        delete t;
}


Thing* Thing::copyInPlace() {
    return new Thing(*this);
}

string Thing::rename(string newName) {
    Thing::things.erase(name);
    _save_name_and_save_in_map(newName);
    return this->name;
}


Point Thing::getCenter() {
    return Point(((2 * position.x) + bounds.right + bounds.left) / 2, ((2 * position.y) + bounds.bottom + bounds.top) / 2);
}

void Thing::manuallyControl(KeyPresses keysDown) {
    if (keysDown.up) 
        position.y-=3;
    if (keysDown.down)
        position.y+=3;
    if (keysDown.left)
        position.x-=3;
    if (keysDown.right)
        position.x+=3;
    if (keysDown.debug_up) 
        position.y--;
    if (keysDown.debug_down)
        position.y++;
    if (keysDown.debug_left)
        position.x--;
    if (keysDown.debug_right)
        position.x++;
    for (auto t : subThings)
        t->manuallyControl(keysDown);
}

void Thing::destroy() {
    Thing::destroyThing(name);
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

vector<Thing*> Thing::findThingsByPoint(Point p) {
    vector<Thing*> matches;
    for (auto const& [id, thing] : Thing::things) {
        if (pointIsInside(p, thing->position, thing->bounds))
            matches.push_back(thing);
    }
    return matches;
}
