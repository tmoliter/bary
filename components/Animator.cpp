#include "Animator.h"

Animator::Animator(Sprite* sprite) : 
    sprite(sprite), 
    type(AnimationType::movement) {
};

void Animator::splitSheet(int columns, int rows) {
    sprite->divideSheet(columns, rows);
    sprite->frontAndCenter();
}

void Animator::animate(Point movement, KeyPresses keysDown) {
    switch(type) {
        case AnimationType::movement:
        default:
            if (movement.x || movement.y) 
                return animateMovementFromSpriteSheet(movement);
            return face(directionFromKeyPresses(keysDown));
    }
}

void Animator::face(Direction d) {
    sprite->d.sourceX = 0;
    switch (d) {
        case (Direction::down):
            sprite->d.sourceY = sprite->d.height * 0;
            break;
        case (Direction::up):
            sprite->d.sourceY = sprite->d.height * 1;
            break;
        case (Direction::left):
            sprite->d.sourceY = sprite->d.height * 2;
            break;
        case (Direction::right):
            sprite->d.sourceY = sprite->d.height * 3;
            break;
        default:
            break;
    };
}

void Animator::animateMovementFromSpriteSheet(Point movement) {
    Direction d = directionFromPoint(movement);
    switch (d) {
        case (Direction::down):
            sprite->d.sourceY = sprite->d.height * 0;
            break;
        case (Direction::up):
            sprite->d.sourceY = sprite->d.height * 1;
            break;
        case (Direction::left):
            sprite->d.sourceY = sprite->d.height * 2;
            break;
        case (Direction::right):
            sprite->d.sourceY = sprite->d.height * 3;
            break;
        case (Direction::none):
            sprite->d.sourceX = 0;
            return;
        default:
            break;
    }
    int speed = d == Direction::up || d == Direction::down ? abs(movement.y) : abs(movement.x);
    int delayPerFrame = 6 / speed;
    int totalFrames = 7;
    int frame = ((frameCount / delayPerFrame) % totalFrames) + 1;
    sprite->d.sourceX = frame * sprite->d.width;
}