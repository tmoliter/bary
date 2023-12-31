#ifndef MOVE_H
#define MOVE_H

#include "Ray.h"

enum class MoveType {
    controlled,
    automatic,
    follow,
    disabled,
};

struct Move {
    Move(MoveType moveType) :
        type(moveType), 
        currentDirection(Direction::down), 
        leader(nullptr),
        speed(1), 
        layer(0),
        tolerance(0),
        destination(Point(-1000,-1000))
    {};

    MoveType type;
    Direction currentDirection;
    Point* leader;

    int speed;
    int layer;
    int tolerance;
    bool disabled = false;
    Point velocity;
    Point destination;
    
    void moveFromInput(KeyPresses keysDown);
    bool autoMove(Point position);

    void changeSpeed(bool decrease);
};

#endif
