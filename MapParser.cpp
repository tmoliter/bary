#include "MapParser.h"

using namespace std;

Thing *parse_thing(ifstream &mapData, ThingList &thingList) {
    char next;
    mapData.get(next);
    if(next == 'T') {
        ThingData newTD;
        Thing::write_thing_datum(mapData, newTD);
        thingList.addThing(new Thing(newTD));
        return thingList.lastThing();
    }
    if(next == 'P') {
        FieldPlayerData newTD;
        FieldPlayer::write_thing_datum(mapData, newTD);
        thingList.addThing(new FieldPlayer(newTD));
        return thingList.lastThing();
    }
    return NULL;
}

// void parse_component(ifstream &mapData, ThingList &thingList) {

// }

void parse_map(ThingList &thingList, Camera *c) {
    ifstream mapData;
    mapData.open("./maps/map.txt");

    Thing *focus = parse_thing(mapData, thingList);

    Camera::parse_camera(mapData, c);

    c->init(&focus->x, &focus->y);

    do {
        parse_thing(mapData, thingList);
        // if(mapData.peek() == ',') {
        //     while(mapData.get() == ',') {
        //         parse_component(mapData,thingList);
        //     }
        // }
    } while (mapData.get() == '\n');
    thingList.initThings(&c->x,&c->y,c->renderer);
    mapData.close();
}