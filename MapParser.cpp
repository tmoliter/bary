#include "MapParser.h"

using namespace std;

void parse_entity(ifstream &mapData, ThingData &newTD) {
    if(mapData.get() == 'T') {
        Thing::write_thing_datum(mapData, newTD);
        return;
    }
}

void parse_map(ThingList &thingList, Camera *c) {
    ifstream mapData;
    mapData.open("./maps/map.txt");

    // GET FOCUS FIRST

    ThingData focusTD;
    parse_entity(mapData, focusTD);
    Thing *focus = new Thing(focusTD);
    thingList.addThing(focus);

    Camera::parse_camera(mapData, c);
    c->init(&focus->x, &focus->y);

    while (mapData.get() == '\n') {
        ThingData newTD;
        char next;
        do {
            parse_entity(mapData, newTD);
            next = mapData.peek();
        } while (next != '\n' && next != EOF);
        thingList.addThing(new Thing(newTD));
    }
    thingList.initThings(&c->x,&c->y,c->renderer);
    mapData.close();
}