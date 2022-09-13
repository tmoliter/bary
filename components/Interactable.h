#ifndef INTERACTABLE_H
#define INTERACTABLE_H
#include "./Collidable.h"
#include "Event.h"
#include <map>

class Interactable : public Collidable {
    public:
        Interactable(Thing* parent, CollidableData cd, Event *e = nullptr);
        ~Interactable();
        int id;

        Event* event;

        static int currentID;
        inline static map<int, Interactable*> interactables;
        static int checkForInteractables(Ray &incoming, int layer);
};

#endif
