#include <vector>
#include <iostream>
#include "Collidable.h"

using namespace std;

Collidable::Collidable (Thing *parent, CollidableData cd) : 
layer(cd.layer),
rays(cd.rays),
active(true),
x(parent->position.x),
y(parent->position.y),
thingName(parent->name) {}

Collidable::~Collidable() {
    for (auto r : rays) {
        delete r;
    }
}

bool Collidable::isColliding(Ray &incoming, int incomingLayer) {
    if (incomingLayer != layer)
        return false;
    for (auto r : rays) {
        Ray ray = Ray(x + r->a.x, y + r->a.y, x + r->b.x, y + r->b.y);
        if (raysCollide(incoming, ray)) {
            return true;
        }
    }
    return false;
}

// STATIC

void _parse_rays (ifstream &mapData, CollidableData &newCD) {
    char next = mapData.peek();
    while(next == 'R') {
            mapData.get();
            mapData.get(next);
            Ray *newRay = new Ray();
            Ray::parse_ray_datum(mapData,*newRay);
            newCD.rays.push_back(newRay);
            next = mapData.peek();
    }
}


int Collidable::parse_collidable_datum(ifstream &mapData, CollidableData &newCD){
    string value = "";
    char current;
    while(mapData.get(current)) {
        if (current == ',') {
            cout << value << endl;
            newCD.layer = std::stoi(value);
            value.clear();
            if (!(mapData.peek() == 'R'))
                break;
            _parse_rays(mapData, newCD);
            cout << endl;
            return 1;
        }
        value.push_back(current);
    }
    return 0;
}
