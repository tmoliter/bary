#include "FieldPlayer.h"
#include <../include/SDL2/SDL_image.h>
#include <iostream>

using namespace std;

FieldPlayer *FieldPlayer::player = nullptr;

FieldPlayer::FieldPlayer(FieldPlayerData fpD) : Thing(fpD) {
    currentDirection = Direction::down;
    sprite = new Sprite(position.x,position.y,name,fpD.spriteData);
    sprite->divideSheet(9, 4);
    walk = new Walk(position.x, position.y, sprite->d.layer, sprite);

    height = sprite->d.height;
    width = sprite->d.width;
    FieldPlayer::player = this;
}

FieldPlayer::~FieldPlayer() { 
    delete sprite;
    delete walk;
    FieldPlayer::player = nullptr;
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

    Direction newDirection = walk->move(dM);
    currentDirection = newDirection == Direction::none ? currentDirection : newDirection;

    if(keysDown.ok && gameState == GameState::FieldFree) {
        int xCenter = position.x + (width / 2);
        int yBottom = position.y + height;
        Ray ray;
        switch (currentDirection) {
            case (Direction::up):
                ray = Ray(xCenter, yBottom, xCenter, yBottom - 8);
                break;
            case (Direction::down):
                ray = Ray(xCenter, yBottom, xCenter, yBottom + 8);
                break;
            case (Direction::left):
                ray = Ray(xCenter, yBottom, xCenter - 8, yBottom);
                break;
            case (Direction::right):
                ray = Ray(xCenter, yBottom, xCenter + 8, yBottom);
                break;
            default:
                ray = Ray(xCenter, yBottom, xCenter, yBottom);
        }
        Interactable::checkForInteractables(ray, sprite->d.layer);
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

    if (keysDown.start) {
        if (Camera::getFocusName() == "Zinnia")
            Camera::panTo("Sailor Shack");
        else
            Camera::panTo("Zinnia");
    }

    if (keysDown.menu1)
        Camera::fadeIn(3);
    if (keysDown.menu2)
        Camera::fadeOut(3);
    /* END DEBUG MODE CONTROLS */        
};

int FieldPlayer::parse_player_datum(ifstream &mapData, FieldPlayerData &newTD) {
    Thing::parse_thing_datum(mapData, newTD);
    SpriteData newSD;
    Sprite::parse_sprite_datum(mapData,newSD);
    newTD.spriteData = newSD;
    return 1;
}
