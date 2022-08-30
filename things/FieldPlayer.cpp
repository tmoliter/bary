#include "FieldPlayer.h"
#include <../include/SDL2/SDL_image.h>
#include <iostream>

using namespace std;

FieldPlayer::FieldPlayer(FieldPlayerData fpD) : Thing(fpD), name(fpD.name) {
    sprite = new Sprite(x,y,id,fpD.spriteData);
    sprite->divideSheet(9, 4);
    walk = new Walk(x,y, sprite->sourceRect);
};

FieldPlayer::~FieldPlayer() { 
    delete sprite;
    delete walk;
};

void FieldPlayer::meat(KeyPresses keysDown) {
    int xV = 0, yV = 0;
    int rayX = x + 16;
    int rayY = y + 32;
    if (keysDown.left) {
        Ray ray = Ray(rayX, rayY, rayX - 8, rayY);
        if (!Obstruction::checkForObstructions(ray))
            xV -= 1;
        }
    if (keysDown.right) {
        Ray ray = Ray(rayX, rayY, rayX + 8, rayY);
        if (!Obstruction::checkForObstructions(ray))
            xV += 1;
        }
    if (keysDown.up) {
        Ray ray = Ray(rayX, rayY, rayX, rayY - 8);
        if (!Obstruction::checkForObstructions(ray))
            yV -= 1;
    }
    if (keysDown.down) {
        Ray ray = Ray(rayX, rayY, rayX, rayY + 8);
        if (!Obstruction::checkForObstructions(ray))
            yV += 1;
        }

    /* DEBUG MODE CONTROLS */
    if (keysDown.debug_left && sprite->layer > 0)
        sprite->layer--;
    if (keysDown.debug_right)
        sprite->layer++;
    if (keysDown.debug_up)
        walk->changeSpeed(false);
    if (keysDown.debug_down)
        walk->changeSpeed(true);
    /* END DEBUG MODE CONTROLS */

    walk->move(xV,yV,x,y);
    walk->animate(xV,yV);

};

int FieldPlayer::write_player_datum(ifstream &mapData, FieldPlayerData &newTD) {
    Thing::write_thing_datum(mapData, newTD);
    SpriteData newSD;
    Sprite::write_sprite_datum(mapData,newSD);
    newTD.spriteData = newSD;
    return 1;
}
