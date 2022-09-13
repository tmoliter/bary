#include "./Obstruction.h"


Obstruction::Obstruction(Thing *parent, CollidableData cd) : Collidable(parent,cd) {
    Obstruction::obstructions[currentID++] = this;
}

Obstruction::~Obstruction() {
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
