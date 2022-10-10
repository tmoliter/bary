#ifndef EVENT_MAP_H
#define EVENT_MAP_H
#include <map>
#include <vector>
#include <string>
#include "./burgEvents.h"
#include "events/Event.h"
#include "components/Collidable.h"

using namespace std;

namespace eventMap {
    inline map<string, Event* (*)()> eventNameToGenerator;
    inline map<pair<string, string>, string> interactableToEventName;
    inline map<pair<string, string>, string> triggerToEventName;
    void buildEventMap();
    void load_events();

    void updateThingName (string oldName, string newName);
    string namePlusEvent(string thing, string collidable, CollidableType type);

    int attachEvent(string event, string thing, string collidable, CollidableType type);
    void removeCollidableEvent(pair<string, string> collidablePair, CollidableType type);
};

#endif
