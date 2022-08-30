#include "globals.h"
#include "Walk.h"
#include <SDL2/SDL.h>
#include <iostream>

using namespace std;

bool Walk::checkCollision(Direction d) {
    vector<Ray> rays;
    switch(d){
        case (Direction::up):
            rays.push_back(Ray(x + 8, y + sourceRect.h, x + 8, y + 24));
            rays.push_back(Ray(x + 24, y + sourceRect.h, x + 24, y + 24));
            break;
        case (Direction::down):
            rays.push_back(Ray(x + 8, y + sourceRect.h, x + 8, y + 40));
            rays.push_back(Ray(x + 24, y + sourceRect.h, x + 24, y + 40));
            break;
        case (Direction::left):
            rays.push_back(Ray(x + 16, y + 40, x + 8, y + 40));
            rays.push_back(Ray(x + 16, y + 24, x + 8, y + 24));
            break;
        case (Direction::right):
            rays.push_back(Ray(x + 16, y + 40, x + 24, y + 40));
            rays.push_back(Ray(x + 16, y + 24, x + 24, y + 24));
            break;
        default:
            break;
    }
    for (auto r : rays) {
        if (Obstruction::checkForObstructions(r))
            return true;
    }
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

void Walk::move(DirectionMap dM){
    Direction d = directionFromMap(dM);
    face(d);
    if (speed == 0) {
        sourceRect.x = 0;
        return;
    }
    if (speed < 3 && frameCount % (3 - speed) != 0) {
        animate(d);
        return;
    }

    int appliedSpeed = speed < 4 ? 1 : speed;
    if (dM.up) {
        if (checkCollision(Direction::up)) 
            dM.up = false;
        else
            y = y - appliedSpeed;
    }
    if (dM.down) {
        if (checkCollision(Direction::down)) 
            dM.up = false;
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
};

void Walk::changeSpeed(bool decrease) {
    if (decrease && speed > 0) {
        speed -= 1;
        return;
    }
    speed +=1;
}