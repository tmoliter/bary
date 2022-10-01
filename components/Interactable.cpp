#include "./Interactable.h"


Interactable::Interactable(Thing* parent, CollidableData cd, Event* e) : Collidable(parent,cd), remaining(-1) {
    event = e;
    event->references++;
    Interactable::interactables[currentID++] = this;
    UIRenderer::addLines(x, y, rays, LineType::interactable);
}

Interactable::~Interactable() {
    for (auto r : rays) {
        UIRenderer::removeLine(r);
    }
    if(--event->references < 1)
        delete event;
    interactables.erase(id);
}

// STATIC

int Interactable::currentID = 0;

int Interactable::checkForInteractables(Ray &incoming, int layer) {
    for (auto const& [id, i] : Interactable::interactables){
        if(i->isColliding(incoming, layer)) {
            if(i->remaining == 0 || !i->event) {
                delete i;
                return 0;
            }
            i->event->begin();
            if (i->remaining)
                i->remaining--;
            return 1;
        }
    }
    return 0;
}
