#ifndef COLLIDABLE_H
#define COLLIDABLE_H
#include "Ray.h"
#include "../things/Thing.h"
#include <vector>
#include <string>
#include <fstream>

using namespace std;

struct CollidableData {
    int layer = 0;
    vector<Ray*> rays;
};

class Collidable {
    public:
        Collidable(Thing *parent, CollidableData cd);
        Collidable(Thing *parent);
        ~Collidable();
        bool active;
        int &x, &y, layer;
        string &thingName;

        vector<Ray*> rays;

        bool isColliding(Ray &incoming, int incomingLayer);

        static int parse_collidable_datum(ifstream &mapData, CollidableData &newCD);
};

#endif
