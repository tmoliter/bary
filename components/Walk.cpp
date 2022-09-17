#include "globals.h"
#include "Walk.h"
#include <SDL2/SDL.h>
#include <iostream>

using namespace std;

void Walk::padSide(DirectionMap dM) {
    Ray ray;
    int xCenter = x + (sprite->d.width / 2);
    int yBottom = y + sprite->d.height;
    if (dM.up || dM.down){
        ray = Ray(xCenter, yBottom, xCenter - 5, yBottom);
        if(Obstruction::checkForObstructions(ray, layer)) 
            x += 1;
        ray = Ray(xCenter, yBottom, xCenter + 5, yBottom);
        if(Obstruction::checkForObstructions(ray, layer)) 
            x -= 1;
    }
    if(dM.left || dM.right) {
        ray = Ray(xCenter, yBottom, xCenter, yBottom - 5);
        if(Obstruction::checkForObstructions(ray, layer)) 
            y += 1;
        ray = Ray(xCenter, yBottom, xCenter, yBottom + 5);
        if(Obstruction::checkForObstructions(ray, layer)) 
            y -= 1;
    }
};


bool Walk::checkCollision(Direction d) {
    Ray ray;
    int xCenter = x + (sprite->d.width / 2);
    int yBottom = y + sprite->d.height;
    switch(d){
        case (Direction::up):
            ray = Ray(xCenter, yBottom, xCenter, yBottom - 6);
            break;
        case (Direction::down):
            ray = Ray(xCenter, yBottom, xCenter, yBottom + 6);
            break;
        case (Direction::left):
            ray = Ray(xCenter, yBottom, xCenter - 6, yBottom);
            break;
        case (Direction::right):
            ray = Ray(xCenter, yBottom, xCenter + 6, yBottom);
            break;
        default:
            return false;
    }
    if (Obstruction::checkForObstructions(ray, layer))
        return true;
    return false;
};

void Walk::animate(Direction d) {
    int totalFrames = 7;
    int delayPerFrame = 12 / speed;
    switch (d) {
        case (Direction::down):
            sprite->d.sourceY = sprite->d.height * 0;
            break;
        case (Direction::up):
            sprite->d.sourceY = sprite->d.height * 1;
            break;
        case (Direction::left):
            sprite->d.sourceY = sprite->d.height * 2;
            break;
        case (Direction::right):
            sprite->d.sourceY = sprite->d.height * 3;
            break;
        case (Direction::none):
            sprite->d.sourceX = 0;
            return;
        default:
            break;
    }
    int frame = ((frameCount / delayPerFrame) % totalFrames) + 1;
    sprite->d.sourceX = frame * sprite->d.width;
}

void Walk::face(Direction d) {
    switch (d) {
        case (Direction::down):
            sprite->d.sourceY = sprite->d.height * 0;
            break;
        case (Direction::up):
            sprite->d.sourceY = sprite->d.height * 1;
            break;
        case (Direction::left):
            sprite->d.sourceY = sprite->d.height * 2;
            break;
        case (Direction::right):
            sprite->d.sourceY = sprite->d.height * 3;
            break;
        default:
            break;
    };
}

Direction Walk::move(DirectionMap dM){
    Direction d = directionFromMap(dM);
    face(d);
    if (speed == 0) {
        sprite->d.sourceX = 0;
        return d;
    }
    if (speed < 3 && frameCount % (3 - speed) != 0) {
        animate(d);
        return d;
    }

    int appliedSpeed = speed < 4 ? 1 : speed;
    padSide(dM);
    if (dM.up) {
        if (checkCollision(Direction::up)) 
            dM.up = false;
        else
            y = y - appliedSpeed;
    }
    if (dM.down) {
        if (checkCollision(Direction::down)) 
            dM.down = false;
        else
            y = y + appliedSpeed;
    }
    if (dM.left) {
        if (checkCollision(Direction::left)) 
            dM.left = false;
        else
            x = x - appliedSpeed;
    }
    if (dM.right) {
        if (checkCollision(Direction::right)) 
            dM.right = false;
        else
            x = x + appliedSpeed;
    }
    animate(directionFromMap(dM));
    return d;
};

void Walk::changeSpeed(bool decrease) {
    if (decrease && speed > 0) {
        speed -= 1;
        return;
    }
    speed +=1;
}