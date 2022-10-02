#include "./Interactable.h"


Interactable::Interactable(Point &pP, string &tN, string n, CollidableData cd, Event* e) : Collidable(pP,tN,cd), name(n), remaining(-1) {
    event = e;
    if (event)
        event->references++;
    Interactable::interactables[currentID++] = this;
    UIRenderer::addLines(parentPos.x, parentPos.y, rays, LineType::interactable);
}


Interactable::Interactable(Point &pP, string &tN, string n, Event *e) : Collidable(pP,tN), name(n), remaining(-1) {
    event = e;
    Interactable::interactables[currentID++] = this;
    UIRenderer::addLines(parentPos.x, parentPos.y, rays, LineType::interactable);
}

Interactable::Interactable(Point &pP, string &tN, string n, vector<Ray*> r, int l, Event *e) : Collidable(pP,tN), name(n), remaining(-1) {
    event = e;
    layer = layer;
    rays = r;
    if (event)
        event->references++;
    Interactable::interactables[currentID++] = this;
    UIRenderer::addLines(parentPos.x, parentPos.y, rays, LineType::interactable);
}

Interactable::~Interactable() {
    for (auto r : rays) {
        UIRenderer::removeLine(r);
    }
    if(event && --event->references < 1)
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
