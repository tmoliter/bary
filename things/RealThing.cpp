#include <../include/SDL2/SDL_image.h>
#include <iostream>
#include "RealThing.h"

using namespace std;

RealThing::RealThing(RealThingData bD) : Thing(bD) {
    for (auto sd : bD.spriteDataVector)
        AddSprite(sd);
    for (auto cd : bD.obstructionData)
        addObstruction(cd.rays, cd.layer);
};

RealThing::RealThing(Point p) : Thing(p) {};
RealThing::RealThing(Point p, string name) : Thing(p, name) {};

RealThing::RealThing(RealThing &oldThing) : Thing(oldThing) {
    for (auto oldS : oldThing.sprites)
        sprites.push_back(new Sprite(*oldS, position, name));
    for (auto const& [layer, oldO] : oldThing.obstructions)
        obstructions[layer] = new Obstruction(*oldO, position, name);
    for (auto const& [oldInName, oldIn] : oldThing.interactables)
        interactables[oldInName] = new Interactable(*oldIn, position, name);
    for (auto const& [oldTrName, oldTr] : oldThing.triggers)
        triggers[oldTrName] = new Trigger(*oldTr, position, name);
}

RealThing::~RealThing() {
    for (auto s : sprites)
        delete s;
    for (auto const& [layer, o] : obstructions)
        delete o;
    for (auto const& [name, in] : interactables)
        delete in;
    for (auto const& [name, tr] : triggers)
        delete tr;
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

Sprite* RealThing::AddSprite(SpriteData SD) {
    sprites.push_back(new Sprite(position, name, SD));
    calculateHeight();
    return sprites.back();
}

Sprite* RealThing::AddRawSprite(string path) {
    SpriteData sd;
    sd.path = path.c_str();
    return AddSprite(sd);
}

Interactable* RealThing::addInteractable(string iName, vector<Ray*> rays, int layer, Event* event) {
    if (interactables.count(iName))
        return interactables[iName];
    Interactable* in = new Interactable(position, name, iName, rays, layer, event);
    interactables[iName] = in;
    return in;
}

Trigger* RealThing::addTrigger(string iName, vector<Ray*> rays, int layer, Event* event) {
    if (triggers.count(iName))
        return triggers[iName];
    Trigger* tr = new Trigger(position, name, iName, rays, layer, event);
    triggers[iName] = tr;
    return tr;
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
    if (interactables.count(iName))
        return interactables[iName];
    Interactable* in = new Interactable(position, name, iName);
    interactables[iName] = in;
    return in;
}

Trigger* RealThing::addTrigger(string iName) {
    if (triggers.count(iName))
        return triggers[iName];
    Trigger* tr = new Trigger(position, name, iName);
    triggers[iName] = tr;
    return tr;
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

void RealThing::removeObstruction(int layer) {
    delete obstructions[layer];
    obstructions.erase(layer);
};

void RealThing::removeInteractable(string name) {
    delete interactables[name];
    interactables.erase(name);
};


void RealThing::removeTrigger(string name) {
    delete triggers[name];
    triggers.erase(name);
};

void RealThing::removeAllCollidables() {
    map<int, Obstruction*>::iterator oItr = obstructions.begin();
    while (oItr != obstructions.end()) {
        delete oItr->second;
        oItr = obstructions.erase(oItr);
    }
    map<string, Interactable*>::iterator inItr = interactables.begin();
    while (inItr != interactables.end()) {
        delete inItr->second;
        inItr = interactables.erase(inItr);
    }
    map<string, Trigger*>::iterator trItr = triggers.begin();
    while (trItr != triggers.end()) {
        delete trItr->second;
        trItr = triggers.erase(trItr);
    }
};

// Pass in incoming Thing name here to ignore collisions
int RealThing::checkForCollidables(Ray incoming, int incomingLayer, CollidableType collidableType) {
    switch (collidableType) {
        case (CollidableType::interactable):
            for (auto const& [name, in] : interactables){
                if(in->isColliding(incoming, incomingLayer)) {
                    if(in->timesTriggered++ == in->maxTriggers) {
                        delete in;
                        interactables.erase(name);
                        return 0;
                    }
                    if (in->event)
                        in->event->begin();
                    return 1;
                }
            }
            break;
        case (CollidableType::trigger):
            for (auto const& [name, tr] : triggers){
                if(tr->isColliding(incoming, incomingLayer)) {
                    if(tr->timesTriggered++ == tr->maxTriggers) {
                        triggers.erase(name);
                        delete tr;
                        return 0;
                    }
                    if (tr->event)
                        tr->event->begin();
                    return 0;
                }
            }
            break;
        case (CollidableType::obstruction):
            for (auto const& [layer, o] : obstructions){
            if(o->isColliding(incoming, incomingLayer))
                return 1;
            }
            break;
        }
    return 0;
}

void RealThing::showObstructionLines(int layer) {
    for (auto const& [l, o] : obstructions) {
        if (layer < -1000 || l == layer)
            o->showLines();
        else 
            o->hideLines();
    }
}

void RealThing::showInteractableLines(int layer, string name) {
    for (auto const& [n, in] : interactables) {
        if ( (layer < -1000 || in->layer == layer) && (name.size() < 1 || n == name) )
            in->showLines();
        else
            in->hideLines();
    }
}

void RealThing::showTriggerLines(int layer, string name) {
    for (auto const& [n, tr] : triggers) {
        if ( (layer < -1000 || tr->layer == layer) && (name.size() < 1 || n == name) )
            tr->showLines();
        else
            tr->hideLines();
    }
}

void RealThing::showLines() {
    showObstructionLines();
    showInteractableLines();
    showTriggerLines();
}

void RealThing::hideObstructionLines() {
    for (auto const& [l, o] : obstructions)
        o->hideLines();
}

void RealThing::hideInteractableLines() {
    for (auto const& [n, in] : interactables)
        in->hideLines();
}

void RealThing::hideTriggerLines() {
    for (auto const& [n, tr] : triggers)
        tr->hideLines();
}


void RealThing::hideLines() {
    hideObstructionLines();
    hideInteractableLines();
    hideTriggerLines();
}

vector<string> RealThing::findAndShowInteractableLines(string beginning) {
    vector<string> matches;
    for (auto const& [name, in] : interactables) {
        if (name.rfind(beginning, 0) == 0) {
            matches.push_back(name);
            in->showLines();
        } else {
            in->hideLines();
        }
    }
    return matches;
}
vector<string> RealThing::findAndShowTriggerLines(string beginning) {
    vector<string> matches;
    for (auto const& [name, tr] : triggers) {
        if (name.rfind(beginning, 0) == 0) {
            matches.push_back(name);
            tr->showLines();
        } else {
            tr->hideLines();
        }
    }
    return matches;
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

Thing* RealThing::copyInPlace() {
    return new RealThing(*this);
}

// STATIC

void RealThing::showAllLines() {
    for (auto const& [id, t] : Thing::things) {
        RealThing* rt = dynamic_cast<RealThing*>(t);
        if (rt)
            rt->showLines();
    }
}

void RealThing::hideAllLines() {
    for (auto const& [id, t] : Thing::things) {
        RealThing* rt = dynamic_cast<RealThing*>(t);
        if (rt)
            rt->hideLines();
    }
}

int RealThing::checkAllObstructions (Ray incoming, int incomingLayer) {
    for (auto const& [n, t] : Thing::things) {
        RealThing* rt = dynamic_cast<RealThing*>(t);
        if (rt && rt->checkForCollidables(incoming, incomingLayer, CollidableType::obstruction))
            return 1;
    }
    return 0;
}

int RealThing::checkAllInteractables (Ray incoming, int incomingLayer) {
    for (auto const& [name, t] : Thing::things) {
        RealThing* rt = dynamic_cast<RealThing*>(t);
        if (rt && rt->checkForCollidables(incoming, incomingLayer, CollidableType::interactable))
            return 1;
    }
    return 0;
}

int RealThing::checkAllTriggers (Ray incoming, int incomingLayer) {
    for (auto const& [name, t] : Thing::things) {
        RealThing* rt = dynamic_cast<RealThing*>(t);
        if (rt && rt->checkForCollidables(incoming, incomingLayer, CollidableType::trigger))
            return 1;
    }
    return 0;
}

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

RealThing* RealThing::findRealThing (string name) {
    return dynamic_cast<RealThing*>(Thing::things[name]);
}
