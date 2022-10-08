#include "eventMap.h"

void eventMap::buildEventMap() {
    eventMap::eventNameToGenerator["test"] = &burgEvents::testEvent;
};

void eventMap::attachEvent(string event, string thing, string collidable) {
    RealThing *rt = RealThing::findRealThing(thing);
    if (!rt)
        return;
    if (!rt->interactables.count(collidable) || !eventMap::eventNameToGenerator.count(event))
        return;
    Event *blah = eventMap::eventNameToGenerator[event]();
    rt->interactables[collidable]->event = blah;
    eventNameToCollidable[event] = make_pair(thing, collidable);
}

void eventMap::load_events() {
    for (auto const& [eventName, thingCollidablePair] : eventNameToCollidable) {
        attachEvent(eventName, thingCollidablePair.first, thingCollidablePair.second);
    }
};
