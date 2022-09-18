#include "MapParser.h"

using namespace std;

Thing *parse_thing(ifstream &mapData) {
    char next;
    mapData.get(next);
    if(next == 'X') {
        cout << next << endl;
        mapData.get();
        return new MapBuilder();
    }
    if(next == 'T') {
        cout << next << endl;
        ThingData newTD;
        Thing::parse_thing_datum(mapData, newTD);
        return new Thing(newTD);
    }
    if(next == 'P') {
        cout << next << endl;
        FieldPlayerData newTD;
        FieldPlayer::parse_player_datum(mapData, newTD);
        return new FieldPlayer(newTD);
    }
    if(next == 'B') {
        cout << next << endl;
        BuildingData newTD;
        Building::parse_building_datum(mapData, newTD);
        return new Building(newTD);
    }
    return NULL;
}

void parse_map(const char *mapPath) {
    ifstream mapData;
    mapData.open(mapPath);
    Thing* focus = parse_thing(mapData);
    new Camera(renderer);
    Camera::parse_camera(mapData);
    Camera::c->init(focus);
    while (mapData.get() == '\n') {
        parse_thing(mapData);
    };
    mapData.close();
}