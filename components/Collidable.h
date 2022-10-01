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
        Collidable (Point &pP, string &tN, CollidableData cd);
        Collidable (Point &pP, string &tN);
        ~Collidable();
        bool active;
        Point &parentPos;
        int layer;
        string &thingName;

        vector<Ray*> rays;

        bool isColliding(Ray &incoming, int incomingLayer);

        static int parse_collidable_datum(ifstream &mapData, CollidableData &newCD);
};

#endif
