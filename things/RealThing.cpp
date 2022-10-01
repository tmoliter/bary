#include "RealThing.h"
#include <../include/SDL2/SDL_image.h>
#include <iostream>

using namespace std;

RealThing::RealThing(RealThingData bD) : Thing(bD) {
    for (auto sd : bD.spriteDataVector) {
        AddSprite(new Sprite(position.x,position.y,name,sd));
    }
    for (auto cd : bD.obstructionData)
        obstructions.push_back(new Obstruction(position, name, cd));
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

void RealThing::calculateHeight() {
    if (sprites.size() == 1) {
        Sprite* sprite = sprites.back();
        bounds.right = sprite->d.width + sprite->d.xOffset;
        bounds.left = sprite->d.xOffset;
        bounds.bottom = sprite->d.height + sprite->d.yOffset;
        bounds.top = sprite->d.yOffset;
        return;
    }
    for (auto s : sprites) {
        int tmpRight = s->d.width + s->d.xOffset;
        int tmpLeft = s->d.xOffset;
        int tmpBottom = s->d.height + s->d.yOffset;
        int tmpTop = s->d.yOffset;
        if (tmpRight > bounds.right)
            bounds.right = tmpRight;
        if (tmpLeft < bounds.left)
            bounds.left = tmpLeft;
        if (tmpBottom > bounds.bottom)
            bounds.bottom = tmpBottom;
        if (tmpTop < bounds.top)
            bounds.top = tmpTop;
    }
}

Sprite* RealThing::AddSprite(Sprite* sprite) {
    sprite->x = position.x;
    sprite->y = position.y;
    sprite->thingName = name;
    sprites.push_back(sprite);
    calculateHeight();
    return sprite;
}

Sprite* RealThing::AddRawSprite(string path) {
    SpriteData sd;
    sd.path = path.c_str();
    return AddSprite(new Sprite(position.x, position.y, name, sd));
}

Interactable* RealThing::addInteractable(vector<Ray*> rays, int layer, Event* event) {
    Interactable* i = new Interactable(position, name, rays, layer, event);
    interactables.push_back(i);
    return i;
}

Obstruction* RealThing::addObstruction(vector<Ray*> rays, int layer) {
    Obstruction* o = new Obstruction(position, name, rays, layer);
    obstructions.push_back(o);
    return o;
}

Interactable* RealThing::addInteractable() {
    Interactable* i = new Interactable(position, name);
    interactables.push_back(i);
    return i;
}

Obstruction* RealThing::addObstruction() {
    Obstruction* o = new Obstruction(position, name);
    obstructions.push_back(o);
    return o;
}

void RealThing::RemoveSprite(Sprite* sprite) {
    delete sprite;
    sprites.erase(remove(sprites.begin(), sprites.end(), sprite), sprites.end());
}

void RealThing::highlightSprite(Sprite* sprite) {
    for (auto s : sprites) {
        if (s != sprite)
            s->alpha = 100;
        else
            s->alpha = 255;
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
