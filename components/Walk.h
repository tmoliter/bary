#ifndef WALK_H
#define WALK_H

#include <vector>
#include "things/RealThing.h"

class Walk {
    private:
        Sprite *sprite;
        int speed;
        
    public:
        int &x, &y, &layer;
        Walk(int &x, int &y, int &layer, Sprite *s) : sprite(s), speed(2), x(x), y(y), layer(layer) {};
        void padSide(DirectionMap dM);
        bool checkCollision(Direction d);
        void animate(Direction d);
        void face(Direction d);
        Direction move(DirectionMap dM);
        void changeSpeed(bool decrease);
};

#endif
