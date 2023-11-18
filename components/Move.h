#ifndef MOVE_H
#define MOVE_H

#include "Ray.h"

enum class MoveType {
    controlled
};

struct Move {
    Move() : type(MoveType::controlled), currentDirection(Direction::down), speed(1), layer(0) {};

    MoveType type;
    Direction currentDirection;

    int speed;
    int layer;
    Point velocity;
    
    void moveFromInput(KeyPresses keysDown);
    void changeSpeed(bool decrease);
};

#endif
