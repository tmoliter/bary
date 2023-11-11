#ifndef ANIMATOR_H
#define ANIMATOR_H
#include "Sprite.h"

enum class AnimationType {
    movement
};

struct Animator {
    Animator(Sprite* sprite);

    Sprite* sprite;
    AnimationType type;

    void splitSheet(int columns, int rows);

    void animate(Point movement, KeyPresses keysDown);
    void face(Direction d); 
    void animateMovementFromSpriteSheet(Point movement);
};

#endif
