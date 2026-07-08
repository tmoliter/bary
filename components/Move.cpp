#include "Move.h"

using namespace std;

Move::Move(Move& oldMove) :
    type(oldMove.type),
    currentDirection(oldMove.currentDirection),
    leaderName(oldMove.leaderName),
    speed(oldMove.speed),
    layer(oldMove.layer),
    tolerance(oldMove.tolerance),
    destination(oldMove.destination) {
}


void Move::moveFromInput(KeyPresses keysDown) {
    if (disables)
        return;
    if (keysDown.up)
        velocity.y -= speed;
    if (keysDown.down)
        velocity.y += speed;
    if (keysDown.left)
        velocity.x -= speed;
    if (keysDown.right)
        velocity.x += speed;
    if (velocity.isNaught())
        return;
    currentDirection = directionFromKeyPresses(keysDown);
}

bool Move::reachedDestination(Point position) {
    return position.isWithin(destination, tolerance);
}

bool Move::autoMove(Point position, Point* leader) {
    if (disables)
        return false;
    if (type == MoveType::follow && leader != nullptr)
        destination = *leader;

    if (reachedDestination(position))
        return true;
    int xDiff = destination.x - position.x;
    int yDiff = destination.y - position.y;
    if (abs(xDiff) < speed)
        velocity.x = xDiff;
    else
        velocity.x = xDiff > 0 ? speed : 0 - speed;
    if (abs(yDiff) <= speed)
        velocity.y = yDiff;
    else
        velocity.y = yDiff > 0 ? speed : 0 - speed;
    return false;
}


void Move::changeSpeed(bool decrease) {
    if (decrease && speed > 0) {
        speed -= 1;
        return;
    }
    speed += 1;
}
