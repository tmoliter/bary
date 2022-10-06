#ifndef INTERACTABLE_H
#define INTERACTABLE_H
#include "Event.h"
#include "./Collidable.h"

struct EventCollidable : public Collidable {
    EventCollidable(Point &pP, string &tN, string n, CollidableData cd, Event *e = nullptr, int maxTriggers = -1);
    EventCollidable(Point &pP, string &tN, string n, vector<Ray*> r, int l, Event *e = nullptr, int maxTriggers = -1);
    EventCollidable(Point &pP, string &tN, string n, Event *e = nullptr, int maxTriggers = -1);
    ~EventCollidable();
    int id, remaining, timesTriggered, maxTriggers;
    string name;
    Event* event;
};

struct Interactable : public EventCollidable {
    Interactable(Point &pP, string &tN, string n, CollidableData cd, Event *e = nullptr, int maxTriggers = -1) : EventCollidable(pP, tN, n, cd, e, maxTriggers) { lineType = LineType::interactable; };
    Interactable(Point &pP, string &tN, string n, vector<Ray*> r, int l, Event *e = nullptr, int maxTriggers = -1) : EventCollidable(pP, tN, n, r, l, e, maxTriggers) { lineType = LineType::interactable; };
    Interactable(Point &pP, string &tN, string n, Event *e = nullptr, int maxTriggers = -1) : EventCollidable(pP, tN, n, e, maxTriggers) { lineType = LineType::interactable; };
};

struct Trigger : public EventCollidable {
    Trigger(Point &pP, string &tN, string n, CollidableData cd, Event *e = nullptr, int maxTriggers = -1) : EventCollidable(pP, tN, n, cd, e, maxTriggers) { lineType = LineType::trigger; };
    Trigger(Point &pP, string &tN, string n, vector<Ray*> r, int l, Event *e = nullptr, int maxTriggers = -1) : EventCollidable(pP, tN, n, r, l, e, maxTriggers) { lineType = LineType::trigger; } ;
    Trigger(Point &pP, string &tN, string n, Event *e = nullptr, int maxTriggers = -1) : EventCollidable(pP, tN, n, e, maxTriggers) { lineType = LineType::trigger; };
};
 
#endif
