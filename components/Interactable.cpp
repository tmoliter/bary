#include "./Interactable.h"


Interactable::Interactable(Thing* parent, CollidableData cd, Event* e) : Collidable(parent,cd) {
    event = e;
    Interactable::interactables[currentID++] = this;
}

Interactable::~Interactable() {
    interactables.erase(id);
}

// STATIC

int Interactable::currentID = 0;

int Interactable::checkForInteractables(Ray &incoming, int layer) {
    for (auto const& [id, o] : Interactable::interactables){
        if(o->isColliding(incoming, layer)) {
            if(o->event)
                o->event->begin();
            return 1;
        }
    }
    return 0;
}
