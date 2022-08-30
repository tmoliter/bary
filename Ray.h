#ifndef RAY_H
#define RAY_H
#include <fstream>
#include <string>
#include <iostream>

using namespace std;

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