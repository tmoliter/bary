#include "./Obstruction.h"


Obstruction::Obstruction(Thing *parent, CollidableData cd) : Collidable(parent,cd) {
    Obstruction::obstructions[currentID++] = this;
    UIRenderer::addLines(x, y, rays, LineType::obstruction);
}

Obstruction::Obstruction(Thing *parent) : Collidable(parent) {
    Obstruction::obstructions[currentID++] = this;
    UIRenderer::addLines(x, y, rays, LineType::obstruction);
}

void Obstruction::addRay(Ray *r) {
    rays.push_back(r);
    UIRenderer::addLine(x, y, r, LineType::obstruction);
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
