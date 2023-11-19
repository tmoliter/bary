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
    Scene::currentScene->things[name] = this;
    Scene::currentScene->AddAnimator(name);
    Scene::currentScene->AddMove(name, MoveType::controlled);
    move->speed = 2;
    FieldPlayer::player = this;
}

void FieldPlayer::meat(KeyPresses keysDown) {
    if (!move->velocity.isNaught())
        Scene::currentScene->checkAllTriggers(getRayFromOriginAndDirection(position, move->currentDirection), move->layer);
    if(keysDown.ok && gameState == GameState::FieldFree)
        Scene::currentScene->checkAllInteractables(getRayFromOriginAndDirection(position, move->currentDirection), move->layer);

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