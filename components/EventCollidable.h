#ifndef INTERACTABLE_H
#define INTERACTABLE_H
#include "./Collidable.h"

struct EventCollidable : public Collidable {
    EventCollidable(Point &pP, string &tN, string n, CollidableData cd = CollidableData()) : Collidable(pP,tN,cd), name(n) {}
    EventCollidable(EventCollidable &oldEC, Point &pP, string &tN) : Collidable(oldEC, pP, tN) {
        if (oldEC.thingName == tN)
            throw exception();
        name = oldEC.name;
    }
    string name; // Unique within parent Thing. Lua binds events to (thingName, name); see event.populate.
};

struct Interactable : public EventCollidable {
    Interactable(Point &pP, string &tN, string n, CollidableData cd  = CollidableData()) : EventCollidable(pP, tN, n, cd) { lineType = LineType::interactable; };
    Interactable(Interactable &oldEC, Point &pP, string &tN) : EventCollidable(oldEC, pP, tN) {};
};

struct Trigger : public EventCollidable {
    Trigger(Point &pP, string &tN, string n, CollidableData cd  = CollidableData()) : EventCollidable(pP, tN, n, cd) { lineType = LineType::trigger; };
    Trigger(Trigger &oldEC, Point &pP, string &tN) : EventCollidable(oldEC, pP, tN) {};
};
 
#endif
