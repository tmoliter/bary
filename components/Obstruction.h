#ifndef OBSTRUCTION_H
#define OBSTRUCTION_H
#include "./Collidable.h"

class Obstruction : Collidable {
    public:
        Obstruction(int &x, int &y, int &tI, CollidableData cd);
        ~Obstruction();

        int id;

        static int currentID;
        inline static map<int, Obstruction*> collidables;
};

#endif
