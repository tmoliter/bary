#include "Ray.h"

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

Ray addPointToRay(Ray ray, Point point) {
    return Ray(point.x + ray.a.x, point.y + ray.a.y, point.x + ray.b.x, point.y + ray.b.y);
};

vector<Ray> getRaysFromOriginAndDirection(Point origin, Direction direction) {
    switch (direction) {
        case (Direction::up):
            return { Ray(origin.x - 16, origin.y, origin.x - 16, origin.y - 16), Ray(origin.x, origin.y, origin.x, origin.y - 16), Ray(origin.x + 16, origin.y, origin.x + 16, origin.y - 16) };
            break;
        case (Direction::down):
            return { Ray(origin.x - 16, origin.y, origin.x - 16, origin.y + 16), Ray(origin.x, origin.y, origin.x, origin.y + 16), Ray(origin.x + 16, origin.y, origin.x + 16, origin.y + 16) };
            break;
        case (Direction::left):
            return { Ray(origin.x, origin.y - 16, origin.x - 16, origin.y - 16 ), Ray(origin.x, origin.y, origin.x - 16, origin.y), Ray(origin.x, origin.y + 16, origin.x - 16, origin.y + 16 ) };
            break;
        case (Direction::right):
            return { Ray(origin.x, origin.y - 16, origin.x + 16, origin.y - 16 ), Ray(origin.x, origin.y, origin.x + 16, origin.y), Ray(origin.x, origin.y + 16, origin.x + 16, origin.y + 16 ) };
            break;
        default:
            return { };
    }
}

Point addPoints(Point point1, Point point2) {
    return Point(point1.x + point2.x, point1.y + point2.y);
}
