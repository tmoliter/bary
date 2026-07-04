#include "FieldPlayer.h"
#include "jukebox.h"

using namespace std;

FieldPlayer *FieldPlayer::player = nullptr;

FieldPlayer::FieldPlayer(RealThingData tD, map<string, RealThing*>& sceneThings) : RealThing(tD, sceneThings) {
    type = ThingType::fieldPlayer;
    AddAnimator();
    AddMove(MoveType::controlled);
    AddStandardCollision();
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
    if(keysDown.ok && !move->disables)
        castRayForInteractables();

    if (keysDown.menu1) {
        loadLuaFunc("beginEvent");
        lua_newtable(L);
        luaUtils::PushStringToTable(L, "eventName", "inventoryMenu");
        luaUtils::PushStringToTable(L, "thingName", "gameManager");
        luaUtils::PushStringToTable(L, "catalyst", "input");
        luaUtils::PushStringToTable(L, "inventoryName", name);
        callLuaFunc(1,0,0);
    }

    /* DEBUG MODE CONTROLS */
    if (keysDown.debug_left)
        shiftLayer(move->layer - 1);
    if (keysDown.debug_right) 
        shiftLayer(move->layer + 1);
    if (keysDown.debug_up)
        move->changeSpeed(false);
    if (keysDown.debug_down)
        move->changeSpeed(true);
    if (keysDown.debug_mute)
        jukebox::toggleMute();
    /* END DEBUG MODE CONTROLS */
    RealThing::meat(keysDown);
};


int FieldPlayer::castRayForInteractables () {
    if (move == nullptr)
        return 0;
    for (auto const& [name, t] : *sceneThings) {
        if (t == this)
            continue;
        for (auto r : getRaysFromOriginAndDirection(position, move->currentDirection))
            if (t->checkForCollidables(r, move->layer, this, CollidableType::interactable))
                return 1;
    }
    return 0;
}

int FieldPlayer::castRayForTriggers () {
    if (move == nullptr)
        return 0;
    for (auto const& [name, t] : *sceneThings) {
        if (t == this)
            continue;
        for (auto r : getRaysFromOriginAndDirection(position, move->currentDirection))
            if (t->checkForCollidables(r, move->layer, this, CollidableType::trigger))
                return 1;
    }
    return 0;
}
