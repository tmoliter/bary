#include "./Obstruction.h"


Obstruction::Obstruction(int &x, int &y, int &tI, CollidableData cd) : Collidable(x,y,tI,cd) {
    id = currentID++;
    Obstruction::obstructions[id] = this;
}

Obstruction::~Obstruction() {
    obstructions.erase(id);
}

// STATIC

int Obstruction::currentID = 0;

bool Obstruction::checkForObstructions(Ray &incoming, int layer) {
    for (auto const& [id, o] : Obstruction::obstructions){
        if(o->isColliding(incoming, layer)) {
            return true;
        }
    }
    return false;
}
