#ifndef COLLIDABLE_H
#define COLLIDABLE_H
#include <map>
#include "Ray.h"
#include <vector>

using namespace std;

struct CollidableData {
    int layer = 0;
    int xOffset = 0;
    int yOffset = 0;
    vector<Ray> rays;
};

class Collidable {
    public:
        Collidable(int &x, int &y, int &tI, CollidableData sd);
        ~Collidable();
        bool active;
        int id, &thingId, &x, &y, layer, yOffset, xOffset;

        void divideSheet(int columns, int rows);
        virtual void render();

        // Push this down into three subtypes
        static int currentID;
        inline static map<int, Collidable*> collidables;

        static int write_collidable_datum(ifstream &mapData, CollidableData &newCD);
};

#endif
