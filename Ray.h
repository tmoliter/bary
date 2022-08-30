#ifndef RAY_H
#define RAY_H

struct Ray {
    int aX;
    int aY;
    int bX;
    int bY;
    Ray (int aX,int aY,int bX,int bY) : aX(aX), aY(aY), bX(bX), bY(bY) {};
    Ray () :  aX(0), aY(0), bX(0), bY(0) {};
};

#endif