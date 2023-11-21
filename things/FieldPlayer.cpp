#include "FieldPlayer.h"

using namespace std;

FieldPlayer *FieldPlayer::player = nullptr;

FieldPlayer::FieldPlayer(RealThingData tD, ThingLists tL) : RealThing(tD, tL) {
    type = ThingType::fieldPlayer;
    AddAnimator();
    AddMove(MoveType::controlled);
    move->speed = 2;
    FieldPlayer::player = this;
}

FieldPlayer::~FieldPlayer() { 
    delete move;
    FieldPlayer::player = nullptr;
};

void FieldPlayer::meat(KeyPresses keysDown) {
    if (!move->velocity.isNaught())
        castRayForTriggers();
    if(keysDown.ok && gameState == GameState::FieldFree)
        castRayForInteractables();

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


int FieldPlayer::castRayForInteractables () {
    if (move == nullptr)
        return 0;
    for (auto const& [name, t] : thingLists.things) {
        if (t == this)
            return 0;
        if (t->checkForCollidables(getRayFromOriginAndDirection(position, move->currentDirection), move->layer, CollidableType::interactable))
            return 1;
    }
    return 0;
}

int FieldPlayer::castRayForTriggers () {
    if (move == nullptr)
        return 0;
    for (auto const& [name, t] : thingLists.things) {
        if (t == this)
            return 0;
        if (t->checkForCollidables(getRayFromOriginAndDirection(position, move->currentDirection), move->layer, CollidableType::trigger))
            return 1;
    }
    return 0;
}