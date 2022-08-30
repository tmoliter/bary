#ifndef WALK_H
#define WALK_H

#include <vector>
#include <../include/SDL2/SDL.h>
#include "../Ray.h"
#include "./Obstruction.h"

class Walk {
    private:
        SDL_Rect &sourceRect;
        int speed;
        
    public:
        int &x, &y;
        Walk(int &x, int &y, SDL_Rect &sourceRect) : sourceRect(sourceRect), speed(2), x(x), y(y) {};
        bool checkCollision(Direction d);
        void animate(Direction d);
        void face(Direction d);
        void move(DirectionMap dM);
        void changeSpeed(bool decrease);
};

#endif
