#ifndef RAY_H
#define RAY_H
#include <tgmath.h>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include "utils.h"

using namespace std;

enum class Direction {
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

struct Bounds {
    int left, right, top, bottom;
    Bounds (int l, int r, int t, int b) : left(l), right(r), top(t), bottom(b) {};
    Bounds () : left(0), right(0), top(0), bottom(0) {};
};

struct Ray {
    Point a;
    Point b;
    Ray (int aX,int aY,int bX,int bY) : a(aX, aY), b(bX, bY) {};
    Ray (Point a, Point b) : a(a), b(b) {};
    Ray (Ray& r) : a(r.a), b(r.b) {};
    Ray () : a(0,0), b(0,0) {};

    string getStringPosition();
};

bool isCounterClockwise (Point A, Point B, Point C);
bool raysCollide(Ray &ray1, Ray &ray2);
bool pointIsInside(Point p, Point boxP, Bounds bounds);

#endif
