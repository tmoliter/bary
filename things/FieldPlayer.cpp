#include "FieldPlayer.h"
#include <../include/SDL2/SDL_image.h>
#include <iostream>

using namespace std;

FieldPlayer::FieldPlayer(FieldPlayerData fpD) : Thing(fpD), name(fpD.name) {
    sprite = new Sprite(x,y,id,fpD.spriteData);
    sprite->divideSheet(9, 4);
    walk = new Walk(x,y, sprite->sourceRect);
};

void FieldPlayer::meat(KeyPresses keysDown) {
    int xV = 0, yV = 0;
    if (keysDown.left)
        xV -= 1;
    if (keysDown.right)
        xV += 1;
    if (keysDown.up)
        yV -= 1;
    if (keysDown.down)
        yV += 1;

    if (keysDown.ok)
        walk->changeSpeed(false);
    if (keysDown.cancel)
        walk->changeSpeed(true);

    walk->move(xV,yV,x,y);
    walk->animate(xV,yV);

    if (keysDown.menu1 && sprite->layer > 0)
        sprite->layer--;
    if (keysDown.menu2)
        sprite->layer++;
};

FieldPlayer::~FieldPlayer() { 
    delete sprite;
    delete walk;
};

int FieldPlayer::write_thing_datum(ifstream &mapData, FieldPlayerData &newTD) {
    Thing::write_thing_datum(mapData, newTD);
    SpriteData newSD;
    Sprite::write_sprite_datum(mapData,newSD);
    newTD.spriteData = newSD;
    return 1;
}
