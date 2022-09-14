#ifndef EVENT_MAP_H
#define EVENT_MAP_H
#include <map>
#include <vector>
#include <string>
#include "./burgEvents.h"

using namespace std;

namespace eventMap {
    inline map<string, vector<void (*)()>> mapping;
    void initialize();
    void load_events(string key);
};

#endif
