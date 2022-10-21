#include "./Interactable.h"


EventCollidable::EventCollidable(Point &pP, string &tN, string n, CollidableData cd, Event* e, int mT) : Collidable(pP,tN,cd), name(n), timesTriggered(0), maxTriggers(mT) {
    event = e;
    if (event)
        event->references++;
}


EventCollidable::EventCollidable(Point &pP, string &tN, string n, Event *e, int mT) : Collidable(pP,tN), name(n), timesTriggered(0), maxTriggers(mT) {
    event = e;
    if (event)
        event->references++;
}

EventCollidable::EventCollidable(Point &pP, string &tN, string n, vector<Ray*> r, int l, Event *e, int mT) : 
    Collidable(pP,tN), name(n), 
    timesTriggered(0), 
    maxTriggers(mT)
    {
    event = e;
    if (event)
        event->references++;
    layer = l;
    rays = r;
    if (event)
        event->references++;
}

EventCollidable::EventCollidable(EventCollidable &oldEC) : Collidable(oldEC) {
    event = oldEC.event;
    if (event)
        event->references++;
    name = oldEC.name + " (Copy)";
}

EventCollidable::~EventCollidable() {
    if(event && --event->references < 1)
        delete event;
}
