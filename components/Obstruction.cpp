#include "./Obstruction.h"


Obstruction::Obstruction(Point &pP, string &tN, CollidableData cd) : Collidable(pP,tN,cd) {
    lineType = LineType::obstruction;
}

Obstruction::Obstruction(Point &pP, string &tN, int l) : Collidable(pP,tN) {
    layer = l;
    lineType = LineType::obstruction;
}

Obstruction::Obstruction(Point &pP, string &tN, vector<Ray*> r, int l) : Collidable(pP,tN) {
    rays = r;
    layer = l;
    lineType = LineType::obstruction;
}
