#include "RealThing.h"

using namespace std;
using namespace luaUtils;

RealThing::RealThing(RealThingData tD) : 
    position(tD.x, tD.y), 
    animator(nullptr),
    move(nullptr) {
    _save_name_and_save_in_map(tD.name);
    for (auto sd : tD.spriteDataVector)
        AddSprite(sd);
    for (auto cd : tD.obstructionData)
        addObstruction(cd.rays, cd.layer);
}

RealThing::RealThing(Point p) : 
    position(p.x,p.y), 
    animator(nullptr),
    move(nullptr) {
    _save_name_and_save_in_map("AnonymousThing");
}
RealThing::RealThing(Point p, string n) : 
    position(p.x,p.y),
    animator(nullptr),
    move(nullptr) {
    _save_name_and_save_in_map(n);
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
    if (animator != nullptr) {
        animatedThings.erase(name);
        delete animator;
    }
    if (move != nullptr) {
        movinThings.erase(name);
        delete move;
    }
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

void RealThing::processMove(KeyPresses keysDown) {
    if (move == nullptr)
        return;
    if (move->type == MoveType::controlled)
        move->moveFromInput(keysDown);
    position.x += move->velocity.x;
    position.y += move->velocity.y;
}

void RealThing::processCollisions() {
    if (move == nullptr)
        return;
    if (move->velocity.x == 0 && move->velocity.y == 0)
        return;
    if (obstructions.count(move->layer) < 1)
        return;
    Obstruction* ownObstruction = obstructions[move->layer];
    for (auto const& [n, thing] : things) {
        if (thing->obstructions.count(move->layer) < 1)
            continue;
        Obstruction* foreignObstruction = thing->obstructions[move->layer];
        if (ownObstruction == foreignObstruction)
            continue;
        for (auto const& ray : foreignObstruction->rays) {
            Ray adjustedRay = addPointToRay(*ray, foreignObstruction->parentPos);
            if(ownObstruction->isColliding(adjustedRay, move->layer)) {
                position.x -= move->velocity.x;
                position.y -= move->velocity.y;
                move->velocity.x = 0;
                move->velocity.y = 0;
                return;
            }
        }
    }
    return;
}

void RealThing::animate(KeyPresses keysDown) {
    animator->animate(move->velocity, keysDown);
    return;
}

void RealThing::meat(KeyPresses keysDown) {
    if (animator != nullptr) {
        animate(keysDown);
    }
    if (move != nullptr) {
        move->velocity.x = 0;
        move->velocity.y = 0;
    }
    return;
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

Animator* RealThing::AddAnimator() {
    // Right now there is only one type of animator, but this could take
    // AnimationType in the future
    if (sprites.size() < 1) {
        std::cout << "Can't animate no sprites!" << std::endl;
        return nullptr;
    }
    animator = new Animator(sprites[0]);
    animator->splitSheet(9, 4); // Obviously this shouldn't be hard-coded, but for now it is
    bounds.bottom = sprites[0]->d.height;
    bounds.right = sprites[0]->d.width;
    animatedThings[name] = this;
    return animator;
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

Obstruction* RealThing::addObstruction(vector<Ray> rays, int layer) {
    Obstruction* o;
    if (obstructions.count(layer)) {
        o = obstructions[layer];
        for (auto r : rays)
            o->addRay(new Ray(r));
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

void RealThing::meatThings(KeyPresses keysDown) {
    for (auto const& [id, thing] : RealThing::movinThings){
        thing->processMove(keysDown);
    }
    for (auto const& [id, thing] : RealThing::movinThings){
        thing->processCollisions();
    }
    for (auto const& [id, thing] : RealThing::things){
        thing->meat(keysDown);
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

RealThingData RealThing::getData() {
    RealThingData td;
    td.name = name;
    td.x = position.x;
    td.y = position.y;
    for (auto s : sprites) {
        td.spriteDataVector.push_back(SpriteData());
        SpriteData& sd = td.spriteDataVector.back();
        sd = s->d;
    }
    for (auto const& [l, o] : obstructions) {
        td.obstructionData.push_back(CollidableData());
        CollidableData& cd = td.obstructionData.back();
        cd.layer = o->layer;
        for (auto r : o->rays) {
            cd.rays.push_back(Ray(*r));
        }
    }
    return td;
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

RealThing* RealThing::findRealThing (string name) {
    return RealThing::things[name];
}

void RealThing::buildThingFromGlobal(lua_State* L) {
    RealThingData td;
    GetLuaIntFromTable(L, "x", td.x);
    GetLuaIntFromTable(L, "y", td.y);
    GetLuaStringFromTable(L, "name", td.name);
    GetTableOnStackFromTable(L, "spriteDataVector");
    if(!lua_isnil(L, -1)) {
        lua_pushnil(L);
        while (lua_next(L, -2)) {
            td.spriteDataVector.push_back(SpriteData());
            SpriteData &newSpriteData = td.spriteDataVector.back();
            GetLuaIntFromTable(L, "height", newSpriteData.height);
            GetLuaIntFromTable(L, "width", newSpriteData.width);
            GetLuaIntFromTable(L, "layer", newSpriteData.layer);
            GetLuaIntFromTable(L, "renderOffset", newSpriteData.renderOffset);
            GetLuaIntFromTable(L, "xOffset", newSpriteData.xOffset);
            GetLuaIntFromTable(L, "yOffset", newSpriteData.yOffset);
            GetLuaIntFromTable(L, "sourceX", newSpriteData.sourceX);
            GetLuaIntFromTable(L, "sourceY", newSpriteData.sourceY);
            GetLuaStringFromTable(L, "textureName", newSpriteData.textureName);
            lua_pop(L, 1);
        }
    }
    lua_pop(L, 1);
    GetTableOnStackFromTable(L, "obstructionData");
    if(!lua_isnil(L, -1)) {
        lua_pushnil(L);
        while (lua_next(L, -2)) {
            td.obstructionData.push_back(CollidableData());
            CollidableData &newObstructionData = td.obstructionData.back();
            GetLuaIntFromTable(L, "layer", newObstructionData.layer);
            GetTableOnStackFromTable(L, "rays");
            lua_pushnil(L);
            while (lua_next(L, -2)) {
                newObstructionData.rays.push_back(Ray());
                Ray& newRay = newObstructionData.rays.back();
                GetLuaIntFromTable(L, "aX", newRay.a.x);
                GetLuaIntFromTable(L, "aY", newRay.a.y);
                GetLuaIntFromTable(L, "bX", newRay.b.x);
                GetLuaIntFromTable(L, "bY", newRay.b.y);
                lua_pop(L, 1);
            }
            lua_pop(L, 2);
        }
        lua_pop(L, 1);
    }
    lua_pop(L, 1);
    new RealThing(td);
}

vector<RealThingData> RealThing::getAllThingData() {
    vector<RealThingData> allData;
    for (auto const& [i, t] : things) {
        if (t->name == "EditorDot")
            continue;
        if (t->name == "test player")
            continue;
        allData.push_back(t->getData());
    }
    return allData;
}