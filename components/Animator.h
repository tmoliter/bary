#ifndef ANIMATOR_H
#define ANIMATOR_H
#include "Sprite.h"

enum class AnimationType {
    movement
};

struct Animator {
    Animator() : type(AnimationType::movement) {};
    Sprite* sprite;
    AnimationType type;
    void animate(Point movement, KeyPresses keysDown);
    void face(Direction d); 
    void animateMovementFromSpriteSheet(Point movement);
};

#endif
