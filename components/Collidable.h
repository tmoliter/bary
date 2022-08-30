#ifndef COLLIDABLE_H
#define COLLIDABLE_H
#include <map>
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
        Collidable(int &x, int &y, int &tI, CollidableData cd);
        ~Collidable();
        bool active;
        int &thingId, &x, &y, layer;

        vector<Ray*> rays;

        bool isColliding(Ray &incoming);

        static int write_collidable_datum(ifstream &mapData, CollidableData &newCD);
};

#endif
