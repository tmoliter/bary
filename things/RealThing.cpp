#include "RealThing.h"
#include <../include/SDL2/SDL_image.h>
#include <iostream>

using namespace std;

RealThing::RealThing(RealThingData bD) : Thing(bD) {
    for (auto sd : bD.spriteDataVector) {
        int tmpWidth = sd.width + sd.xOffset;
        if (tmpWidth > width)
            width = tmpWidth;
        int tmpHeight = sd.height + sd.yOffset;
        if (tmpHeight > height)
            height = tmpHeight;
        sprites.push_back(new Sprite(position.x,position.y,name,sd));
    }
    for (auto cd : bD.obstructionData)
        obstructions.push_back(new Obstruction(this,cd));
};

RealThing::RealThing(Point p) : Thing(p) {};
RealThing::RealThing(Point p, string name) : Thing(p, name) {};

RealThing::~RealThing() {
    for (auto s : sprites)
        delete s;
    for (auto o : obstructions)
        delete o;
};

void RealThing::_save_name_and_save_in_map(string n) {
    Thing::_save_name_and_save_in_map(n);
    for (auto s : sprites)
        s->thingName = name;
}

Sprite* RealThing::AddSprite(Sprite* sprite) {
    sprite->x = position.x;
    sprite->y = position.y;
    sprite->thingName = name;
    int tmpWidth = sprite->d.width + sprite->d.xOffset;
    int tmpHeight = sprite->d.height + sprite->d.yOffset;
    if (tmpWidth > sprite->d.width)
        width = tmpWidth;
    if (tmpHeight > sprite->d.height)
        height = tmpHeight;
    sprites.push_back(sprite);
    return sprite;
}

Sprite* RealThing::AddRawSprite(string path) {
    SpriteData sd;
    sd.path = path.c_str();
    return AddSprite(new Sprite(position.x, position.y, name, sd));
}

void RealThing::highlightSprite(Sprite* sprite) {
    for (auto s : sprites) {
        if (s != sprite)
            s->alpha = 100;
    }
}

void RealThing::removeHighlight() {
    for (auto s : sprites)
        s->alpha = 255;
}

// STATIC

int RealThing::parse_building_datum(ifstream &mapData, RealThingData &newTD) {
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

RealThing* RealThing::find_building (string name) {
    return dynamic_cast<RealThing*>(Thing::things[name]);
}
