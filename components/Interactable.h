#ifndef INTERACTABLE_H
#define INTERACTABLE_H
#include <map>
#include <string>
#include "./Collidable.h"
#include "Event.h"
#include "gui/UIRenderer.h"
#include "gui/Line.h"

class Interactable : public Collidable {
    public:
        Interactable(Point &pP, string &tN, string n, CollidableData cd, Event *e = nullptr);
        Interactable(Point &pP, string &tN, string n, vector<Ray*> r, int l, Event *e = nullptr);
        Interactable(Point &pP, string &tN, string n, Event *e = nullptr);
        ~Interactable();
        int id, remaining;
        string name;
        Event* event;

        static int currentID;
        inline static map<int, Interactable*> interactables;
        static int checkForInteractables(Ray &incoming, int layer);
};

#endif
