#ifndef MOVE_H
#define MOVE_H

#include "Ray.h"

enum class MoveType {
    controlled,
    automatic,
    follow,
    disabled
};

struct Move {
    Move(MoveType moveType, Point origin) :
        type(moveType), 
        origin(origin), 
        destination(origin),
        currentDirection(Direction::down), 
        leader(nullptr),
        speed(1), 
        layer(0)
    {};

    MoveType type;
    Direction currentDirection;
    Point* leader;

    int speed;
    int layer;
    Point velocity;
    Point origin;
    Point destination;
    
    void moveFromInput(KeyPresses keysDown);
    bool autoMove(Point position);

    void changeSpeed(bool decrease);
};

#endif
