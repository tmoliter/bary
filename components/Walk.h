#ifndef WALK_H
#define WALK_H
// #include <map>

#include <../include/SDL2/SDL.h>

// using namespace std;

class Walk {
    private:
        SDL_Rect &sourceRect;
        int speed;
        
    public:
        Walk(int &x, int &y, SDL_Rect &sourceRect) : sourceRect(sourceRect), speed(2) {};
        void move(int xV, int yV, int &x, int &y, int tick);
        void animate(int x, int y, int tick);
        void changeSpeed(bool decrease);
        // static map<int, Walk*> objects;
};

#endif
