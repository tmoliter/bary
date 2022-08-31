#ifndef RAY_H
#define RAY_H
#include <fstream>
#include <string>
#include <iostream>

using namespace std;

enum Direction {
    up,
    down,
    left,
    right,
    none
};

struct DirectionMap {
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
    DirectionMap () : up(false), down(false), left(false), right(false) {};
};

inline Direction directionFromMap(DirectionMap dM) {
    if (dM.up) 
        return Direction::up;
    if (dM.down) 
        return Direction::down;
    if (dM.left) 
        return Direction::left;
    if (dM.right) 
        return Direction::right;
    return Direction::none;
}


struct Point {
    int x;
    int y;
    Point (int x, int y) : x(x), y(y) {};
    Point () : x(0), y(0) {};
};

struct Ray {
    Point a;
    Point b;
    Ray (int aX,int aY,int bX,int bY) : a(aX, aY), b(bX, bY) {};
    Ray () : a(0,0), b(0,0) {};

   static int write_ray_datum(ifstream &mapData, Ray &newRay);
};

bool isCounterClockwise (Point A, Point B, Point C);
bool raysCollide(Ray &ray1, Ray &ray2);

#endif
