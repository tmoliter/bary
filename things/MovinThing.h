#ifndef MOVIN_THING_H
#define MOVIN_THING_H

#include <iostream>
#include "RealThing.h"

struct MovinThingData : RealThingData {
};

struct MovinThing : public RealThing {
    void init();

    Point velocity;
    Direction facing;
    MovinThing(MovinThingData fpD);
    MovinThing(Point p, string n, string textureName);
    ~MovinThing();

    Direction currentDirection;
    Sprite *sprite;

    void getRay(Ray &r);

    void meat(KeyPresses keysDown);
    
    static MovinThing *player;
};

#endif
