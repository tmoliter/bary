#include "eventMap.h"

void eventMap::initialize() {
    eventMap::mapping["Burg"].push_back(&burgEvents::sailor_shack_test);
};

void eventMap::load_events(string key) {
    vector<void (*)()> events = eventMap::mapping[key];
    for (auto f : events) {
        f();
    }
};