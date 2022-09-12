#include "Timer.h"

Timer::Timer() : start(frameCount) {};

void Timer::restart() { start = frameCount;};

int Timer::getTime() { return frameCount - start;};

void Timer::setTimeSinceStart(int delta) { start += delta; };


// STATIC

void Timer::startOrRestart(string key) { 
    if(timers.count(key))
        timers[key]->restart();
    else
        timers[key] = new Timer();
};

void Timer::startOrIgnore(string key) { 
    if(!timers.count(key))
        timers[key] = new Timer();
};

int Timer::timeSince(string key) {
    if(timers.count(key))
        return timers[key]->getTime();
    return -1;
};

void Timer::destroy(string key) {
    if(timers.count(key)) {
        delete timers[key];
        timers.erase(key);
    }
};
