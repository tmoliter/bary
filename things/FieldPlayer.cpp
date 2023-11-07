#include "FieldPlayer.h"

using namespace std;

FieldPlayer *FieldPlayer::player = nullptr;

FieldPlayer::FieldPlayer(FieldPlayerData fpD) : RealThing(fpD) {
    init();
}

FieldPlayer::FieldPlayer(Point p, string name, string textureName) : RealThing(p,name) {
    AddRawSprite(textureName);
    init();
}

FieldPlayer::~FieldPlayer() { 
    delete walk;
    FieldPlayer::player = nullptr;
};

void FieldPlayer::init() {
    currentDirection = Direction::down;
    sprite = sprites[0];
    sprite->divideSheet(9, 4);
    sprite->frontAndCenter();
    walk = new Walk(position.x, position.y, sprite->d.layer, sprite);

    bounds.bottom = sprite->d.height;
    bounds.right = sprite->d.width;
    FieldPlayer::player = this;
}


void FieldPlayer::getRay(Ray &r) {
    switch (currentDirection) {
        case (Direction::up):
            r = Ray(position.x, position.y, position.x, position.y - 16);
            break;
        case (Direction::down):
            r = Ray(position.x, position.y, position.x, position.y + 16);
            break;
        case (Direction::left):
            r = Ray(position.x, position.y, position.x - 16, position.y);
            break;
        case (Direction::right):
            r = Ray(position.x, position.y, position.x + 16, position.y);
            break;
        default:
            r = Ray(position.x, position.y, position.x, position.y);
    }
}

void FieldPlayer::meat(KeyPresses keysDown) {
    int xV = 0, yV = 0;
    DirectionMap dM;
    vector<Ray> rv;
    if (keysDown.up) 
        dM.up = true;
    if (keysDown.down)
        dM.down = true;
    if (keysDown.left)
        dM.left = true;
    if (keysDown.right)
        dM.right = true;

    Direction newDirection = walk->move(dM);
    currentDirection = newDirection == Direction::none ? currentDirection : newDirection;

    Ray ray;
    getRay(ray);
    // This can totally be hoisted if it makes more sense, but maybe not?
    RealThing::checkAllTriggers(ray, sprite->d.layer);
    if(keysDown.ok && gameState == GameState::FieldFree) {
        RealThing::checkAllInteractables(ray, sprite->d.layer);
    }

    /* DEBUG MODE CONTROLS */
    if (keysDown.debug_left && sprite->d.layer > 0)
        sprite->d.layer--;
    if (keysDown.debug_right)
        sprite->d.layer++;
    if (keysDown.debug_up)
        walk->changeSpeed(false);
    if (keysDown.debug_down)
        walk->changeSpeed(true);

    // There's no real reason that any of this should live in field player. In face,
    // Field player could only be responsible for literally moving around and
    // not running into things
    // if (keysDown.menu1)
    //     Camera::fadeIn(3);
    // if (keysDown.menu2)
    //     Camera::fadeOut(3);
    /* END DEBUG MODE CONTROLS */
};