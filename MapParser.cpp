#include "MapParser.h"

using namespace std;

void parse_map() {
    MapBuilder *m = new MapBuilder();
    RealThing* focus = m->currentThing;
    new Camera(renderer);
    new FocusTracker();
    FocusTracker::ftracker->setFocus(focus);
    Camera::c->path = "./assets/backgrounds/Burg.png";
    Camera::c->init();
    RealThing::buildThingFromGlobal("timmytesty.lua");
}