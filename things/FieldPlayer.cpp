#include "FieldPlayer.h"

using namespace std;

FieldPlayer *FieldPlayer::player = nullptr;

FieldPlayer::FieldPlayer(FieldPlayerData fpD) : RealThing(fpD) {
    init();
}

FieldPlayer::FieldPlayer(Point p, string n, string textureName) : RealThing(p,n) {
    AddRawSprite(textureName);
    init();
}

FieldPlayer::~FieldPlayer() { 
    delete move;
    FieldPlayer::player = nullptr;
};

void FieldPlayer::init() {
    AddAnimator();
    AddMove();
    FieldPlayer::player = this;
}

void FieldPlayer::meat(KeyPresses keysDown) {
    Ray ray;
    switch (directionFromPoint(move->velocity)) {
        case (Direction::up):
            ray = Ray(position.x, position.y, position.x, position.y - 16);
            break;
        case (Direction::down):
            ray = Ray(position.x, position.y, position.x, position.y + 16);
            break;
        case (Direction::left):
            ray = Ray(position.x, position.y, position.x - 16, position.y);
            break;
        case (Direction::right):
            ray = Ray(position.x, position.y, position.x + 16, position.y);
            break;
        default:
            ray = Ray(position.x, position.y, position.x, position.y);
    }
    RealThing::checkAllTriggers(ray, move->layer);
    if(keysDown.ok && gameState == GameState::FieldFree) {
        RealThing::checkAllInteractables(ray, move->layer);
    }

    /* DEBUG MODE CONTROLS */
    if (keysDown.debug_left && sprites[0]->d.layer > 0) {
        // This feels pretty hacky, but I guess we could put it in a function on RealThing like this.
        // It shouldn't happen too much.
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