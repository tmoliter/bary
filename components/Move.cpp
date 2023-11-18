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


void Move::changeSpeed(bool decrease) {
    if (decrease && speed > 0) {
        speed -= 1;
        return;
    }
    speed += 1;
}