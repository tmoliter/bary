#include "MapParser.h"

using namespace std;

Thing *parse_thing(ifstream &mapData) {
    char next;
    mapData.get(next);
    if(next == 'T') {
        cout << next << endl;
        ThingData newTD;
        Thing::write_thing_datum(mapData, newTD);
        return new Thing(newTD);
    }
    if(next == 'P') {
        cout << next << endl;
        FieldPlayerData newTD;
        FieldPlayer::write_player_datum(mapData, newTD);
        return new FieldPlayer(newTD);
    }
    if(next == 'B') {
        cout << next << endl;
        BuildingData newTD;
        Building::write_building_datum(mapData, newTD);
        return new Building(newTD);
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