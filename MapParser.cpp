#include "MapParser.h"

using namespace std;

void parse_entity(ifstream &mapData, Thing *newThing) {
    if(mapData.get() == 'T') {
        Thing::write_thing_datum(mapData, newThing);
        return;
    }
}

void parse_map(ThingList &thingList) {
    ifstream mapData; 
    mapData.open("./maps/map.txt");

    // while (mapData.get(current)) {
    //  do background stuff
    // }
    while (mapData.get() == '\n') {
        Thing *newThing;
        char next;
        do {
            parse_entity(mapData, newThing);
            next = mapData.peek();
        } while (next != '\n' && next != EOF);
        thingList.AddThing(newThing);
    }

    mapData.close();
}