#include <vector>
#include <iostream>
#include "Collidable.h"

using namespace std;

Collidable::Collidable (int &x, int &y, int &tI, CollidableData cd) : 
x(x), 
y(y), 
thingId(tI),
layer(cd.layer),
rays(cd.rays),
active(true) {}

Collidable::~Collidable() {
    for (auto r : rays) {
        delete r;
    }
}

bool Collidable::isColliding(Ray &incoming) {
    for (auto r : rays) {
        if (raysCollide(incoming, *r)) {
            return true;
        }
    }
    return false;
}

// STATIC

void _write_rays (ifstream &mapData, CollidableData &newCD) {
    char next = mapData.peek();
    while(next != '\n' && next != EOF) {
        if(next == 'R'){
            mapData.get();
            mapData.get(next);
        }
        Ray newRay;
        Ray::write_ray_datum(mapData,newRay);
        newCD.rays.push_back(&newRay);
        next = mapData.peek();
    }
} 


int Collidable::write_collidable_datum(ifstream &mapData, CollidableData &newCD){
    int index = 0;
    string value = "";
    char current;
    while(mapData.get(current)) {
        if (current == ',' || current == ' ') {
            switch(index) {
                case (0):
                    cout << value << endl;
                    newCD.layer = std::stoi(value);
                    value.clear();
                    index++;
                    break;
                case (1):
                    _write_rays(mapData, newCD);
                    return 1;
                default:
                    return 0;
            }
            continue;
        }
        value.push_back(current);
    }
    return 1;
}