#include "FieldPlayer.h"
#include <../include/SDL2/SDL_image.h>
#include <iostream>

using namespace std;

FieldPlayer::FieldPlayer(FieldPlayerData fpD) : Thing(fpD) {
    sprite = new Sprite(position.x,position.y,name,fpD.spriteData);
    sprite->divideSheet(9, 4);
    walk = new Walk(position.x, position.y, sprite->layer, sprite->sourceRect);

    height = sprite->height;
    width = sprite->width;
    name = "Zinnia";
};

FieldPlayer::~FieldPlayer() { 
    delete sprite;
    delete walk;
};

void FieldPlayer::meat(KeyPresses keysDown) {
    int xV = 0, yV = 0;
    int rayX = position.x + 16;
    int rayY = position.y + 32;
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

    walk->move(dM);

    /* DEBUG MODE CONTROLS */
    if (keysDown.debug_left && sprite->layer > 0)
        sprite->layer--;
    if (keysDown.debug_right)
        sprite->layer++;
    if (keysDown.debug_up)
        walk->changeSpeed(false);
    if (keysDown.debug_down)
        walk->changeSpeed(true);

    if (keysDown.debug_return) {
        if (Camera::getFocusName() == "Zinnia")
            Camera::panTo("Sailor Shack");
        else
            Camera::panTo("Zinnia");
    }

    if (keysDown.debug_plus)
        Camera::fadeIn(5);
    if (keysDown.debug_minus)
        Camera::fadeOut(5);
    /* END DEBUG MODE CONTROLS */        
};

int FieldPlayer::write_player_datum(ifstream &mapData, FieldPlayerData &newTD) {
    Thing::write_thing_datum(mapData, newTD);
    SpriteData newSD;
    Sprite::write_sprite_datum(mapData,newSD);
    newTD.spriteData = newSD;
    return 1;
}
