#include "./Obstruction.h"


Obstruction::Obstruction(Point &pP, string &tN, CollidableData cd) : Collidable(pP,tN,cd) {
    Obstruction::obstructions[currentID++] = this;
    UIRenderer::addLines(parentPos.x, parentPos.y, rays, LineType::obstruction);
}

Obstruction::Obstruction(Point &pP, string &tN, int l) : Collidable(pP,tN) {
    layer = l;
    Obstruction::obstructions[currentID++] = this;
    UIRenderer::addLines(parentPos.x, parentPos.y, rays, LineType::obstruction);
}

Obstruction::Obstruction(Point &pP, string &tN, vector<Ray*> r, int l) : Collidable(pP,tN) {
    rays = r;
    layer = l;
    Obstruction::obstructions[currentID++] = this;
    UIRenderer::addLines(parentPos.x, parentPos.y, rays, LineType::obstruction);
}

void Obstruction::addRay(Ray *r) {
    rays.push_back(r);
    UIRenderer::addLine(parentPos.x, parentPos.y, r, LineType::obstruction);
}

Obstruction::~Obstruction() {
    for (auto r : rays)
    UIRenderer::removeLine(r);
    obstructions.erase(id);
}

// STATIC

int Obstruction::currentID = 0;

int Obstruction::checkForObstructions(Ray &incoming, int layer) {
    for (auto const& [id, o] : Obstruction::obstructions){
        if(o->isColliding(incoming, layer)) {
            return 1;
        }
    }
    return 0;
}
