#ifndef INTERACTABLE_H
#define INTERACTABLE_H
#include "./Collidable.h"
#include "Event.h"
#include "gui/UIRenderer.h"
#include "gui/Line.h"
#include <map>

class Interactable : public Collidable {
    public:
        Interactable(Thing* parent, CollidableData cd, Event *e = nullptr);
        ~Interactable();
        int id, remaining;
        Event* event;

        static int currentID;
        inline static map<int, Interactable*> interactables;
        static int checkForInteractables(Ray &incoming, int layer);
};

#endif
