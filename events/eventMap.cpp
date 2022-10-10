#include "eventMap.h"

void eventMap::buildEventMap() {
    eventMap::eventNameToGenerator["test"] = &burgEvents::testEvent;
    eventMap::eventNameToGenerator["fake"] = nullptr;
};

int eventMap::attachEvent(string event, string thing, string collidable, CollidableType type) {
    RealThing *rt = RealThing::findRealThing(thing);
    if (!rt)
        return 0;
    if (type == CollidableType::obstruction)
        return 0;
    if (type == CollidableType::trigger && !rt->triggers.count(collidable))
        return 0;
    if (type == CollidableType::interactable && !rt->interactables.count(collidable))
        return 0;
    if (!eventMap::eventNameToGenerator.count(event))
        return 0;
    if (eventMap::eventNameToGenerator[event] == nullptr)
        return 0;
    Event *blah = eventMap::eventNameToGenerator[event]();
    if (type == CollidableType::trigger) {
        rt->triggers[collidable]->event = blah;
        triggerToEventName[make_pair(thing, collidable)] = event;
    }
    if (type == CollidableType::interactable) {
        rt->interactables[collidable]->event = blah;
        interactableToEventName[make_pair(thing, collidable)] = event;
    }
    return 1;
}

void eventMap::removeCollidableEvent(pair<string, string> collidablePair, CollidableType type) {
    if (type == CollidableType::trigger && triggerToEventName.count(collidablePair)) {
        eventMap::triggerToEventName.erase(collidablePair);
    }
    if (type == CollidableType::interactable && interactableToEventName.count(collidablePair)) {
        eventMap::interactableToEventName.erase(collidablePair);
    }
}

void eventMap::load_events() {
    for (auto const& [thingCollidablePair, eventName] : triggerToEventName)
        attachEvent(eventName, thingCollidablePair.first, thingCollidablePair.second, CollidableType::trigger);
    for (auto const& [thingCollidablePair, eventName] : interactableToEventName)
        attachEvent(eventName, thingCollidablePair.first, thingCollidablePair.second, CollidableType::interactable);
};

void eventMap::updateThingName (string oldName, string newName) {
    vector<pair<string,string>> toDelete;
    for (auto const& [collidablePair, eventName] : triggerToEventName) {
        if (collidablePair.first == oldName) {
            triggerToEventName[make_pair(newName, collidablePair.second)] = eventName;
            toDelete.push_back(collidablePair);
        }
    }
    for (auto deletion : toDelete)
        triggerToEventName.erase(deletion);
}

string eventMap::namePlusEvent(string thing, string collidable, CollidableType type) {
    string name = collidable;
    pair<string,string> collidableNames = make_pair(thing, collidable);
    if (type == CollidableType::interactable && eventMap::interactableToEventName.count(collidableNames))
        name = name + " (event name: " + eventMap::interactableToEventName[collidableNames] + ")";
    if (type == CollidableType::trigger && eventMap::triggerToEventName.count(collidableNames))
        name = name + " (event name: " + eventMap::triggerToEventName[collidableNames] + ")";
    return name;
}