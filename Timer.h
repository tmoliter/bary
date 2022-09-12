#ifndef TIMER_H
#define TIMER_H
#include <string>
#include <map>
#include "globals.h"

using namespace std;

struct Timer {
    int start;

    Timer();

    void restart();
    int getTime();
    void setTimeSinceStart(int delta);
    
    inline static map<string, Timer*> timers;
    static void startOrRestart(string key);
    static void startOrIgnore(string key);
    static int timeSince(string key);
    static void destroy(string key);

};

#endif
