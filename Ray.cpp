#include "Ray.h"

int Ray::write_ray_datum(ifstream &mapData, Ray &newRay) {
    int index = 0;
    string value = "";
    char current;
    while(mapData.get(current)) {
        if (current == ',' || current == '>') {
            switch(index) {
                case (0):
                    cout << value << endl;
                    newRay.a.x = std::stoi(value);
                    value.clear();
                    index++;
                    break;
                case (1):
                    cout << value << endl;
                    newRay.a.y = std::stoi(value);
                    value.clear();
                    index++;
                    break;
                case (2):
                    cout << value << endl;
                    newRay.b.x = std::stoi(value);
                    value.clear();
                    index++;
                    break;
                case (3):
                    cout << value << endl;
                    newRay.b.y = std::stoi(value);
                    value.clear();
                    index++;
                    return 1;
                default:
                    return 0;
            }
            continue;
        }
        value.push_back(current);
    }
    return 0;
}

bool isCounterClockwise (Point A, Point B, Point C) {  
    return (C.y-A.y)*(B.x-A.x) > (B.y-A.y)*(C.x-A.x);
};

bool raysCollide(Ray &ray1, Ray &ray2) {
    return (
        isCounterClockwise(ray1.a,ray1.b,ray2.a) != isCounterClockwise(ray1.a,ray1.b,ray2.b) &&
        isCounterClockwise(ray2.a,ray2.b,ray1.a) != isCounterClockwise(ray2.a,ray2.b,ray1.b)
    );
};