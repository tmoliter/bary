#include <vector>
#include <iostream>
#include "Collidable.h"

using namespace std;

Collidable::Collidable (Point &pP, string &tN, CollidableData cd) : 
layer(cd.layer),
active(true),
parentPos(pP),
thingName(tN),
linesVisible(false) {
    for (auto r : cd.rays) {
        this->addRay(new Ray(r));
    }
}

Collidable::Collidable (Point &pP, string &tN) : 
layer(0),
active(true),
parentPos(pP),
thingName(tN),
linesVisible(false) {}

Collidable::Collidable (Collidable &oldCollidable, Point &pP, string &tN) : // Could we simplify this to just take the first param?
thingName(tN),
parentPos(pP),
layer(oldCollidable.layer),
active(oldCollidable.active),
linesVisible(oldCollidable.linesVisible),
lineType(oldCollidable.lineType) {
    for (auto r : oldCollidable.rays)
        rays.push_back(new Ray(*r));
}

Collidable::~Collidable() {
    for (auto r : rays) {
        UIRenderer::removeLine(r);
        delete r;
    }
}

void Collidable::addRay(Ray *r) {
    rays.push_back(r);
    if (linesVisible)
        UIRenderer::addLine(parentPos.x, parentPos.y, r, lineType);
}

void Collidable::removeRay(int index) {
    if (rays.size() < index + 1)
        return;
    UIRenderer::removeLine(rays[index]);
    delete rays[index];
    rays.erase(rays.begin() + index);
}

void Collidable::showLines() {
    if(linesVisible)
        return;
    linesVisible = true;
    for (auto r : rays)
        UIRenderer::addLine(parentPos.x, parentPos.y, r, lineType);
}

void Collidable::hideLines() {
    if(!linesVisible)
        return;
    linesVisible = false;
    for (auto r : rays)
        UIRenderer::removeLine(r);
}


bool Collidable::isColliding(Ray &incoming, int incomingLayer) {
    if (incomingLayer != layer)
        return false;
    for (auto r : rays) {
        Ray ray = addPointToRay(*r, parentPos);
        if (raysCollide(incoming, ray))
            return true;
    }
    return false;
}
