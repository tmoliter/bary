#include "Ray.h"

int Ray::parse_ray_datum(ifstream &mapData, Ray &newRay) {
    utils::parse_ints(vector<int*> { &newRay.a.x, &newRay.a.y, &newRay.b.x, &newRay.b.y }, mapData);
    return 1;
}

string Ray::getStringPosition() {
    double angle = atan2((b.y - a.y), (b.x - a.x));
    double degrees = angle * (180.0/3.1415927);
    double rounded = floor(degrees * 100 + .5) / 100;
    string trimmed = to_string(rounded).substr(0, std::to_string(rounded).find(".") + 3);
    return to_string(a.x) + "," + to_string(a.y) + " : " + to_string(b.x) + "," + to_string(b.y) + " (" + trimmed + "deg)";
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

bool pointIsInside(Point p, Point boxP, Bounds bounds) {
    if (p.x < boxP.x + bounds.left || p.x > boxP.x + bounds.right || p.y < boxP.y + bounds.top || p.y > boxP.y + bounds.bottom)
        return false;
    return true;
}