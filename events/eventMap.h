#ifndef EVENT_MAP_H
#define EVENT_MAP_H
#include <map>
#include <vector>
#include <string>
#include "./burgEvents.h"
#include "Event.h"

using namespace std;

namespace eventMap {
    inline map<string, Event* (*)()> eventNameToGenerator;
    inline map<string, pair<string, string>> eventNameToCollidable;
    void buildEventMap();
    void load_events();

    void attachEvent(string event, string thing, string collidable);
};

#endif
