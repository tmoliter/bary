#ifndef OBSTRUCTION_H
#define OBSTRUCTION_H
#include "./Collidable.h"
#include "gui/Line.h"
#include "gui/UIRenderer.h"
#include <map>

class Obstruction : public Collidable {
    public:
        Obstruction(Point &pP, string &tN, CollidableData cd);
        Obstruction(Point &pP, string &tN, vector<Ray*> r, int l);
        Obstruction(Point &pP, string &tN);
        ~Obstruction();
        int id;

        void addRay(Ray *r);

        static int currentID;
        inline static map<int, Obstruction*> obstructions;
        static int checkForObstructions(Ray &incoming, int layer);
};

#endif
