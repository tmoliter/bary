#ifndef RAY_H
#define RAY_H
#include <tgmath.h>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include "utils.h"
#include "Input.h"

using namespace std;

enum class Direction {
    up,
    down,
    left,
    right,
    none
};

struct Point {
    int x;
    int y;
    Point (int x, int y) : x(x), y(y) {};
    Point () : x(0), y(0) {};
    bool isNaught() {
        return x == 0 && y == 0;
    }
    bool isWithin(Point otherPoint, int tolerance) {
        int xDiff = abs(this->x - otherPoint.x);
        int yDiff = abs(this->y - otherPoint.y);
        if (xDiff + yDiff > tolerance)
            return false;
        return true;
    }
    bool operator==(const Point& other) {
        return this->x == other.x && this->y == other.y;
    }
    bool operator!=(const Point& other) {
        return !(*this == other);
    }
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

inline Direction directionFromPoint(Point vector) {
    if (vector.y < 0) 
        return Direction::up;
    if (vector.y > 0) 
        return Direction::down;
    if (vector.x < 0) 
        return Direction::left;
    if (vector.x > 0) 
        return Direction::right;
    return Direction::none;
}

inline Direction directionFromKeyPresses(KeyPresses keysDown) {
    if (keysDown.up) 
        return Direction::up;
    if (keysDown.down) 
        return Direction::down;
    if (keysDown.left) 
        return Direction::left;
    if (keysDown.right) 
        return Direction::right;
    return Direction::none;
}

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
    Ray (const Ray& r) : a(r.a), b(r.b) {};
    Ray () : a(0,0), b(0,0) {};

    string getStringPosition();
};

bool isCounterClockwise (Point A, Point B, Point C);
bool raysCollide(Ray &ray1, Ray &ray2);
bool pointIsInside(Point p, Point boxP, Bounds bounds);
Ray getRayFromOriginAndDirection(Point origin, Direction direction);
Ray addPointToRay(Ray ray, Point point);

#endif
