#include "Building.h"
#include <../include/SDL2/SDL_image.h>
#include <iostream>

using namespace std;

Building::Building(BuildingData bD) : Thing(bD), name(bD.name) {
    for (auto sd : bD.spriteDataVector) {
        sprites.push_back(new Sprite(x,y,id,sd));
    }
};

Building::~Building() { 
    for (auto s : sprites) {
            delete s;
        }
};

int Building::write_thing_datum(ifstream &mapData, BuildingData &newTD) {
    Thing::write_thing_datum(mapData, newTD);
    while(mapData.peek() != '\n' && mapData.peek() != EOF) {
        SpriteData newSD;
        Sprite::write_sprite_datum(mapData,newSD);
        newTD.spriteDataVector.push_back(newSD);
    }

    return 1;
}
