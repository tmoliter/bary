#include "./Obstruction.h"


Obstruction::Obstruction(Point &pP, string &tN, CollidableData cd) : Collidable(pP,tN,cd) {
    lineType = LineType::obstruction;
    Obstruction::obstructions[currentID++] = this;
}

Obstruction::Obstruction(Point &pP, string &tN, int l) : Collidable(pP,tN) {
    layer = l;
    lineType = LineType::obstruction;
    Obstruction::obstructions[currentID++] = this;
}

Obstruction::Obstruction(Point &pP, string &tN, vector<Ray*> r, int l) : Collidable(pP,tN) {
    rays = r;
    layer = l;
    lineType = LineType::obstruction;
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
