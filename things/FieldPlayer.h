#ifndef FIELD_PLAYER_H
#define FIELD_PLAYER_H

#include "Thing.h"
#include "../components/Walk.h"
#include <iostream>



class FieldPlayer : public Thing {
    private:
        Walk* walk;
    public:
        FieldPlayer(int &x, int &y, int *cX, int *cY, SDL_Renderer* renderer) : 
        Thing(x, y, cX, cY, renderer) {
            walk = new Walk(x,y, sourceRect);
        };
        void init() {};

        void premeat() {};
        void meat() {};
        void meat(KeyPresses keysDown);
        void destroy();
};
#endif
