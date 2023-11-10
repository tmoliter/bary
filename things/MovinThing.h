#ifndef MOVIN_THING_H
#define MOVIN_THING_H

#include <iostream>
#include "components/Move.h"

struct MovinThingData : RealThingData {
};

struct MovinThing : public RealThing {
    string name;
    void init();

    Move* walk;
    Point velocity;
    Direction facing;
    MovinThing(MovinThingData fpD);
    MovinThing(Point p, string name, string textureName);
    ~MovinThing();

    Direction currentDirection;
    Sprite *sprite;

    void getRay(Ray &r);

    void move(KeyPresses keysDown);
    void meat(KeyPresses keysDown);
    
    static MovinThing *player;
};

#endif
