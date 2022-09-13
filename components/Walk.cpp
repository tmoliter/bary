#include "globals.h"
#include "Walk.h"
#include <SDL2/SDL.h>
#include <iostream>

using namespace std;

void Walk::padSide(DirectionMap dM) {
    Ray ray;
    int xCenter = x + (sourceRect.w / 2);
    int yBottom = y + sourceRect.h;
    if (dM.up || dM.down){
        ray = Ray(xCenter, yBottom, xCenter - 7, yBottom);
        if(Obstruction::checkForObstructions(ray, layer)) 
            x += 1;
        ray = Ray(xCenter, yBottom, xCenter + 7, yBottom);
        if(Obstruction::checkForObstructions(ray, layer)) 
            x -= 1;
    }
    if(dM.left || dM.right) {
        ray = Ray(xCenter, yBottom, xCenter, yBottom - 7);
        if(Obstruction::checkForObstructions(ray, layer)) 
            y += 1;
        ray = Ray(xCenter, yBottom, xCenter, yBottom + 7);
        if(Obstruction::checkForObstructions(ray, layer)) 
            y -= 1;
    }
};


bool Walk::checkCollision(Direction d) {
    Ray ray;
    int xCenter = x + (sourceRect.w / 2);
    int yBottom = y + sourceRect.h;
    switch(d){
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
            sourceRect.y = sourceRect.h * 0;
            break;
        case (Direction::up):
            sourceRect.y = sourceRect.h * 1;
            break;
        case (Direction::left):
            sourceRect.y = sourceRect.h * 2;
            break;
        case (Direction::right):
            sourceRect.y = sourceRect.h * 3;
            break;
        case (Direction::none):
            sourceRect.x = 0;
            return;
        default:
            break;
    }
    int frame = ((frameCount / delayPerFrame) % totalFrames) + 1;
    sourceRect.x = frame * sourceRect.w;
}

void Walk::face(Direction d) {
    switch (d) {
        case (Direction::down):
            sourceRect.y = sourceRect.h * 0;
            break;
        case (Direction::up):
            sourceRect.y = sourceRect.h * 1;
            break;
        case (Direction::left):
            sourceRect.y = sourceRect.h * 2;
            break;
        case (Direction::right):
            sourceRect.y = sourceRect.h * 3;
            break;
        default:
            break;
    };
}

Direction Walk::move(DirectionMap dM){
    Direction d = directionFromMap(dM);
    face(d);
    if (speed == 0) {
        sourceRect.x = 0;
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