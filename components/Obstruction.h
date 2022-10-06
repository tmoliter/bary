#ifndef OBSTRUCTION_H
#define OBSTRUCTION_H
#include "./Collidable.h"

class Obstruction : public Collidable {
    public:
        Obstruction(Point &pP, string &tN, CollidableData cd);
        Obstruction(Point &pP, string &tN, vector<Ray*> r, int l);
        Obstruction(Point &pP, string &tN, int l);
};

#endif
