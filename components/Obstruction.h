#ifndef OBSTRUCTION_H
#define OBSTRUCTION_H
#include "./Collidable.h"

struct Obstruction : public Collidable {
    Obstruction(Point &pP, string &tN, CollidableData cd);
    Obstruction(Point &pP, string &tN, vector<Ray*> r, int l);
    Obstruction(Point &pP, string &tN, int l);
};

#endif
