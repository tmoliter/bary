#include "MovinThing.h"

using namespace std;

MovinThing *MovinThing::player = nullptr;

MovinThing::MovinThing(MovinThingData fpD) : RealThing(fpD) {
    init();
}

MovinThing::MovinThing(Point p, string n, string textureName) : RealThing(p,n) {
    AddRawSprite(textureName);
    init();
}

MovinThing::~MovinThing() { 
    delete move;
    MovinThing::player = nullptr;
};

void MovinThing::init() {
    RealThing::movinThings[name] = this;
    AddAnimator();
    bounds.top = 0;
    bounds.bottom = 0;
    bounds.right = 0 - ( sprites[0]->d.width / 2) + 8;
    bounds.left = (sprites[0]->d.width / 2) - 10;

    move = new Move();
    MovinThing::player = this;

    vector<Ray> obstructionRays = {
        Ray(Point(bounds.left, bounds.bottom), Point(bounds.right, bounds.bottom)),
        Ray(Point(bounds.right, bounds.bottom), Point(bounds.right, bounds.bottom - 6)),
        Ray( Point(bounds.right, bounds.bottom - 6), Point(bounds.left, bounds.bottom - 6)),
        Ray(Point(bounds.left, bounds.bottom - 6), Point(bounds.left, bounds.bottom))
    };
    addObstruction(obstructionRays, 0);


    currentDirection = Direction::down;
}


void MovinThing::getRay(Ray &r) {
    switch (directionFromPoint(velocity)) {
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

void MovinThing::meat(KeyPresses keysDown) {
    if (velocity.x != 0 || velocity.y != 0)
        currentDirection = directionFromPoint(velocity);

    Ray ray;
    getRay(ray);
    // This can totally be hoisted if it makes more sense, but maybe not?
    RealThing::checkAllTriggers(ray, move->layer);
    if(keysDown.ok && gameState == GameState::FieldFree) {
        RealThing::checkAllInteractables(ray, move->layer);
    }

    /* DEBUG MODE CONTROLS */
    if (keysDown.debug_left && sprites[0]->d.layer > 0) {
        // This feels pretty hacky, but I guess we could put it in a function. It shouldn't happen too much.
        int oldLayer = move->layer;
        int newLayer = oldLayer - 1;
        move->layer = newLayer;
        sprites[0]->d.layer = newLayer;

        Obstruction* ob = obstructions[oldLayer];
        ob->layer = newLayer;
        obstructions[ob->layer] = ob;
        obstructions.erase(oldLayer);
    }
    if (keysDown.debug_right) {
        int oldLayer = move->layer;
        int newLayer = oldLayer + 1;
        move->layer = newLayer;
        sprites[0]->d.layer = newLayer;

        Obstruction* ob = obstructions[oldLayer];
        ob->layer = newLayer;
        obstructions[ob->layer] = ob;
        obstructions.erase(oldLayer);
    }
    if (keysDown.debug_up)
        move->changeSpeed(false);
    if (keysDown.debug_down)
        move->changeSpeed(true);
    /* END DEBUG MODE CONTROLS */
    RealThing::meat(keysDown);
};