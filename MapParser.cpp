#include "MapParser.h"

using namespace std;

Thing *parse_thing(ifstream &mapData) {
    char next;
    mapData.get(next);
    if(next == 'T') {
        ThingData newTD;
        Thing::write_thing_datum(mapData, newTD);
        return new Thing(newTD);
    }
    if(next == 'P') {
        FieldPlayerData newTD;
        FieldPlayer::write_thing_datum(mapData, newTD);
        return new FieldPlayer(newTD);
    }
    return NULL;
}

void parse_map(const char *mapPath) {
    ifstream mapData;
    mapData.open(mapPath);
    Thing* focus = parse_thing(mapData);
    Camera::parse_camera(mapData);
    Camera::c->init(&focus->x, &focus->y);
    while (mapData.get() == '\n') {
        parse_thing(mapData);
    };
    mapData.close();
}