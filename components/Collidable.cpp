#include <vector>
#include <iostream>
#include "Collidable.h"

using namespace std;

Collidable::Collidable (Point &pP, string &tN, CollidableData cd) : 
layer(cd.layer),
rays(cd.rays),
active(true),
parentPos(pP),
thingName(tN) {}

Collidable::Collidable (Point &pP, string &tN) : 
layer(0),
active(true),
parentPos(pP),
thingName(tN) {}

Collidable::~Collidable() {
    for (auto r : rays) {
        delete r;
    }
}

bool Collidable::isColliding(Ray &incoming, int incomingLayer) {
    if (incomingLayer != layer)
        return false;
    for (auto r : rays) {
        Ray ray = Ray(parentPos.x + r->a.x, parentPos.y + r->a.y, parentPos.x + r->b.x, parentPos.y + r->b.y);
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
            newCD.layer = std::stoi(value);
            value.clear();
            if (!(mapData.peek() == 'R'))
                break;
            _parse_rays(mapData, newCD);
            return 1;
        }
        value.push_back(current);
    }
    return 0;
}
