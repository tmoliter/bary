#include "RealThing.h"

using namespace std;

RealThing::RealThing(RealThingData tD) : position(tD.x, tD.y) {
    _save_name_and_save_in_map(tD.name);
    for (auto sd : tD.spriteDataVector)
        AddSprite(sd);
    for (auto cd : tD.obstructionData)
        addObstruction(cd.rays, cd.layer);
}

RealThing::RealThing(Point p) : position(p.x,p.y) {
    _save_name_and_save_in_map("AnonymousThing");
}
RealThing::RealThing(Point p, string name) : 
    position(p.x,p.y),
    name(name) {
    _save_name_and_save_in_map(name);
}

RealThing::RealThing(RealThing &oldThing) : position(oldThing.position), bounds(oldThing.bounds) {
    string n = oldThing.name;
    _save_name_and_save_in_map(n);
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
    for (auto t : subThings)
        delete t;
    things.erase(name); // This came before deleting subThings in original Thing.cpp, beware if bug arises
};

void RealThing::_save_name_and_save_in_map(string n) {
    int i;
    for (i = 0; i < n.length(); i++)
        if (isdigit(n[i]))
            break;
    string baseName = n.substr(0, i);
    i = 1;
    name = baseName;
    while(RealThing::things.count(name)) {
        name = baseName + to_string(i);
        i++;
    }
    RealThing::things[name] = this;
    for (auto s : sprites) // REFACTOR Sprite shouldn't have to care about this
        s->thingName = name;
}

void RealThing::calculateHeight() { // Do we need to save this to bounds and risk it being out of date? Do some code reading
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
    sprites.back()->active = true; // This was added to test lua loading, might have side effects
    calculateHeight();
    return sprites.back();
}

Sprite* RealThing::AddRawSprite(string textureName) {
    SpriteData sd;
    sd.textureName = textureName;
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

RealThing* RealThing::copyInPlace() {
    return new RealThing(*this);
}

// Ported Over from Thing.cpp
string RealThing::rename(string newName) {
    RealThing::things.erase(name);
    _save_name_and_save_in_map(newName);
    return this->name;
}

Point RealThing::getCenter() {
    return Point(((2 * position.x) + bounds.right + bounds.left) / 2, ((2 * position.y) + bounds.bottom + bounds.top) / 2);
}

void RealThing::manuallyControl(KeyPresses keysDown) {
    if (keysDown.up) 
        position.y-=3;
    if (keysDown.down)
        position.y+=3;
    if (keysDown.left)
        position.x-=3;
    if (keysDown.right)
        position.x+=3;
    if (keysDown.debug_up) 
        position.y--;
    if (keysDown.debug_down)
        position.y++;
    if (keysDown.debug_left)
        position.x--;
    if (keysDown.debug_right)
        position.x++;
    for (auto t : subThings)
        t->manuallyControl(keysDown);
}


void RealThing::destroy() {
    RealThing::destroyThing(name);
}

// STATIC PORTED OVER FROM Thing.cpp

int RealThing::parse_thing_datum(ifstream &mapData, RealThingData &newTD) {
    mapData.get();
    utils::parse_strings(vector <string*> { &newTD.name }, mapData);
    utils::parse_ints(vector <int*> { &newTD.x, &newTD.y }, mapData);
    return 1;
}

void RealThing::meatThings(KeyPresses keysDown) {
    for (auto const& [id, thing] : RealThing::things){
        thing->meat(keysDown);
        thing->meat();
    }
}

void RealThing::destroyThings() {
    for (auto killMe : thingsToDestroy) {
        RealThing* thing = RealThing::things[killMe];
        delete thing;
    }
    thingsToDestroy.clear();
}

void RealThing::destroyAllThings() {
   map<string, RealThing*>::iterator itr = RealThing::things.begin();
   while (itr != RealThing::things.end()) {
        delete itr->second;
        itr = RealThing::things.erase(itr);
   }
}

void RealThing::destroyThing(string n) {
    thingsToDestroy.push_back(n);
}

vector<RealThing*> RealThing::findThingsByPoint(Point p) {
    vector<RealThing*> matches;
    for (auto const& [id, thing] : RealThing::things) {
        if (pointIsInside(p, thing->position, thing->bounds))
            matches.push_back(thing);
    }
    return matches;
}

// STATIC

void RealThing::showAllLines() {
    for (auto const& [id, t] : RealThing::things) {
        t->showLines();
    }
}

void RealThing::hideAllLines() {
    for (auto const& [id, t] : RealThing::things) {
        t->hideLines();
    }
}

int RealThing::checkAllObstructions (Ray incoming, int incomingLayer) {
    for (auto const& [n, t] : RealThing::things) {
        if (t->checkForCollidables(incoming, incomingLayer, CollidableType::obstruction))
            return 1;
    }
    return 0;
}

int RealThing::checkAllInteractables (Ray incoming, int incomingLayer) {
    for (auto const& [name, t] : RealThing::things) {
        if (t->checkForCollidables(incoming, incomingLayer, CollidableType::interactable))
            return 1;
    }
    return 0;
}

int RealThing::checkAllTriggers (Ray incoming, int incomingLayer) {
    for (auto const& [name, t] : RealThing::things) {
        if (t->checkForCollidables(incoming, incomingLayer, CollidableType::trigger))
            return 1;
    }
    return 0;
}

int RealThing::parse_building_datum(ifstream &mapData, RealThingData &newTD) {
    RealThing::parse_thing_datum(mapData, newTD);
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
    return dynamic_cast<RealThing*>(RealThing::things[name]);
}
