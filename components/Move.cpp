#include "Move.h"

using namespace std;

void Move::moveFromInput(KeyPresses keysDown) {
    if (keysDown.up)
        velocity.y -= speed;
    if (keysDown.down)
        velocity.y += speed;
    if (keysDown.left)
        velocity.x -= speed;
    if (keysDown.right)
        velocity.x += speed;
    currentDirection = directionFromKeyPresses(keysDown);
}

bool Move::autoMove(Point position) {
    if (position == destination)
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
