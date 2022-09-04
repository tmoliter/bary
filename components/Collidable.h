#ifndef COLLIDABLE_H
#define COLLIDABLE_H
#include "Ray.h"
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
        Collidable(int &x, int &y, string &tN, CollidableData cd);
        ~Collidable();
        bool active;
        int &x, &y, layer;
        string &thingName;

        vector<Ray*> rays;

        bool isColliding(Ray &incoming, int incomingLayer);

        static int write_collidable_datum(ifstream &mapData, CollidableData &newCD);
};

#endif
