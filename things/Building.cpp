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
        sprites.push_back(new Sprite(position.x,position.y,name,sd));
    }
    for (auto cd : bD.obstructionData) {
        obstructions.push_back(new Obstruction(this,cd));
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

int Building::parse_building_datum(ifstream &mapData, BuildingData &newTD) {
    Thing::parse_thing_datum(mapData, newTD);
    char next = mapData.peek();
    while(next != '\n' && next != EOF) {
        if(next == 'S'){
            mapData.get();
            mapData.get();
            SpriteData newSD;
            Sprite::parse_sprite_datum(mapData,newSD);
            newTD.spriteDataVector.push_back(newSD);
            next = mapData.peek();
            continue;
        }
        if(next == 'O'){
            mapData.get();
            mapData.get();
            CollidableData newCD;
            Collidable::parse_collidable_datum(mapData,newCD);
            newTD.obstructionData.push_back(newCD);
            next = mapData.peek();
            continue;
        }
        return 0;
    }

    return 1;
}

Building* Building::find_building (string name) {
    return dynamic_cast<Building*>(Thing::things[name]);
}
