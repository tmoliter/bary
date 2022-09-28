#ifndef OBSTRUCTION_H
#define OBSTRUCTION_H
#include "./Collidable.h"
#include "gui/Line.h"
#include "gui/UIRenderer.h"
#include <map>

class Obstruction : public Collidable {
    public:
        Obstruction(Thing *parent, CollidableData cd);
        ~Obstruction();
        int id;

        static int currentID;
        inline static map<int, Obstruction*> obstructions;
        static int checkForObstructions(Ray &incoming, int layer);
};

#endif
