#include "MapParser.h"

using namespace std;

RealThing *parse_thing(ifstream &mapData) {
    char next;
    mapData.get(next);
    if(next == 'X') {
        if(SCALE != 1) {
            cout << "Map Builder can only be used if SCALE == 1" << endl;
            throw exception();
        }
        mapData.get();
        MapBuilder *m = new MapBuilder();
        return m->currentThing;
    }
    if(next == 'T') {
        RealThingData newTD;
        RealThing::parse_thing_datum(mapData, newTD);
        return new RealThing(newTD);
    }
    if(next == 'P') {
        FieldPlayerData newTD;
        FieldPlayer::parse_player_datum(mapData, newTD);
        return new FieldPlayer(newTD);
    }
    if(next == 'B') {
        RealThingData newTD;
        RealThing::parse_building_datum(mapData, newTD);
        return new RealThing(newTD);
    }
    return NULL;
}

void parse_map(const char *mapPath) {
    ifstream mapData;
    mapData.open(mapPath);
    RealThing* focus = parse_thing(mapData);
    new Camera(renderer);
    new FocusTracker();
    Camera::parse_camera(mapData);
    FocusTracker::ftracker->setFocus(focus);
    Camera::c->init();
    while (mapData.get() == '\n') {
        parse_thing(mapData);
    };
    mapData.close();
}