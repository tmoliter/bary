#include "Ray.h"

int Ray::parse_ray_datum(ifstream &mapData, Ray &newRay) {
    utils::parse_ints(vector<int*> { &newRay.a.x, &newRay.a.y, &newRay.b.x, &newRay.b.y }, mapData);
    return 1;
}

bool isCounterClockwise (Point A, Point B, Point C) {  
    return (C.y-A.y)*(B.x-A.x) > (B.y-A.y)*(C.x-A.x);
};

bool raysCollide(Ray &ray1, Ray &ray2) {
    return (
        isCounterClockwise(ray1.a,ray1.b,ray2.a)  != isCounterClockwise(ray1.a,ray1.b,ray2.b) &&
        isCounterClockwise(ray2.a,ray2.b,ray1.a)!= isCounterClockwise(ray2.a,ray2.b,ray1.b)
    );
};
