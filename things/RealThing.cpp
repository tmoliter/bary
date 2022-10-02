#include "RealThing.h"
#include <../include/SDL2/SDL_image.h>
#include <iostream>

using namespace std;

RealThing::RealThing(RealThingData bD) : Thing(bD) {
    for (auto sd : bD.spriteDataVector) {
        AddSprite(new Sprite(position.x,position.y,name,sd));
    }
    for (auto cd : bD.obstructionData)
        addObstruction(cd.rays, cd.layer);
};

RealThing::RealThing(Point p) : Thing(p) {};
RealThing::RealThing(Point p, string name) : Thing(p, name) {};

RealThing::~RealThing() {
    for (auto s : sprites)
        delete s;
    for (auto const& [layer, o] : obstructions)
        removeObstruction(o->layer);
    for (auto const& [name, in] : interactables)
        removeInteractable(in->name);
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

Interactable* RealThing::addInteractable(string iName, vector<Ray*> rays, int layer, Event* event) {
    Interactable* in;
    int i = 2;
    string tmpName = iName;
    while (interactables.count(tmpName)) {
        tmpName = iName + to_string(i);
        i++;
    }
    in = new Interactable(position, tmpName, iName, rays, layer, event);
    interactables[tmpName] = in;
    return in;
}

Obstruction* RealThing::addObstruction(vector<Ray*> rays, int layer) {
    Obstruction* o;
    if (obstructions.count(layer)) {
        o = obstructions[layer];
        for (auto r : rays)
            o->addRay(r);
    }
    else {
        o = new Obstruction(position, name, rays, layer);
        obstructions[layer] = o;
    }
    return o;
}

Interactable* RealThing::addInteractable(string iName) {
    int i = 2;
    string tmpName = iName;
    while (interactables.count(tmpName)) {
        tmpName = iName + to_string(i);
        i++;
    }
    Interactable* in = new Interactable(position, name, tmpName);
    interactables[tmpName] = in;
    return in;
}

Obstruction* RealThing::addObstruction(int layer) {
    if (obstructions.count(layer)) {
        return obstructions[layer];
    }
    Obstruction *o = new Obstruction(position, name, layer);
    obstructions[layer] = o;
    return o;
}

void RealThing::RemoveSprite(Sprite* sprite) {
    delete sprite;
    sprites.erase(remove(sprites.begin(), sprites.end(), sprite), sprites.end());
}

void RealThing::removeInteractable(string name) {
    delete interactables[name];
    interactables.erase(name);
};

void RealThing::removeObstruction(int layer) {
    delete obstructions[layer];
    obstructions.erase(layer);
};

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
