#include "Building.h"
#include <../include/SDL2/SDL_image.h>
#include <iostream>

using namespace std;

Building::Building(BuildingData bD) : Thing(bD) {
    for (auto sd : bD.spriteDataVector) {
        int tmpWidth = sd.width + sd.xOffset;
        if (tmpWidth > width)
            width = tmpWidth;
        int tmpHeight = sd.height + sd.yOffset;
        if (tmpHeight > height)
            height = tmpHeight;
        cout << "BLD HEIGHT: " << height << endl;
        sprites.push_back(new Sprite(position.x,position.y,id,sd));
    }
    for (auto bd : bD.obstructionData) {
        obstructions.push_back(new Obstruction(position.x,position.y,id,bd));
    }
};

Building::~Building() {
    for (auto s : sprites) {
        delete s;
    }
    for (auto o : obstructions) {
        delete o;
    }
};

int Building::write_building_datum(ifstream &mapData, BuildingData &newTD) {
    Thing::write_thing_datum(mapData, newTD);
    char next = mapData.peek();
    while(next != '\n' && next != EOF) {
        if(next == 'S'){
            mapData.get();
            mapData.get();
            SpriteData newSD;
            Sprite::write_sprite_datum(mapData,newSD);
            newTD.spriteDataVector.push_back(newSD);
            next = mapData.peek();
            continue;
        }
        if(next == 'O'){
            mapData.get();
            mapData.get();
            CollidableData newCD;
            Collidable::write_collidable_datum(mapData,newCD);
            newTD.obstructionData.push_back(newCD);
            next = mapData.peek();
            continue;
        }
        return 0;
    }

    return 1;
}
