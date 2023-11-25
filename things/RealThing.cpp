#include "RealThing.h"

RealThing::RealThing(RealThingData tD, ThingLists tL) :
    name(tD.name),
    position(tD.x, tD.y), 
    animator(nullptr),
    move(nullptr),
    thingLists(tL) {
    for (auto sd : tD.spriteDataVector)
        AddSprite(sd);
    for (auto cd : tD.obstructionData)
        addObstruction(cd.rays, cd.layer);
}

RealThing::RealThing(RealThing &oldThing) : position(oldThing.position), bounds(oldThing.bounds), thingLists(oldThing.thingLists) {
    for (auto oldS : oldThing.sprites)
        sprites.push_back(new Sprite(*oldS, position, name));
    for (auto const& [layer, oldO] : oldThing.obstructions)
        obstructions[layer] = new Obstruction(*oldO, position, name);
    for (auto const& [oldInName, oldIn] : oldThing.interactables)
        interactables[oldInName] = new Interactable(*oldIn, position, name);
    for (auto const& [oldTrName, oldTr] : oldThing.triggers)
        triggers[oldTrName] = new Trigger(*oldTr, position, name);
    parentScene = oldThing.parentScene;
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

// void RealThing::callLuaFunc(lua_State *L, int nargs, int nresults, int errfunc) {
//     lua_pushlightuserdata(L, parentScene);
//     Host::callLuaFunc(L, nargs + 1, nresults, errfunc);
// }

void RealThing::processMove(KeyPresses keysDown) {
    if (move == nullptr)
        return;
    if (move->type == MoveType::controlled)
        move->moveFromInput(keysDown);
    if (move->type == MoveType::follow && move->leader)
        move->autoMove(position);
    if (move->type == MoveType::automatic) {
        if(move->autoMove(position)) {
            loadLuaFunc(sceneL, "doAutoMove");
            callLuaFunc(sceneL, 0, 0, 0);
        }
    }
    position.x += move->velocity.x;
    position.y += move->velocity.y;
}

void RealThing::processCollisions(map<string, RealThing*>& things) {
    if (move == nullptr)
        return;
    if (move->velocity.x == 0 && move->velocity.y == 0)
        return;
    if (obstructions.count(move->layer) < 1)
        return;
    Obstruction* ownObstruction = obstructions[move->layer];
    for (auto const& [n, thing] : things) {
        if (thing == this)
            continue;
        if (thing->obstructions.count(move->layer) < 1)
            continue;
        Obstruction* foreignObstruction = thing->obstructions[move->layer];
        // NOTE ABOUT PERFORMANCE:
        // With 41 player characters walking around and two static things,
        // this hovers around 2.5ms and sometimes spikes around 6ms. If performance becomes a concern,
        // we can try a couple things:
        // maybe play with boxes + rays? Idea being we could do less calculations if we treat
        // colliders that are boxes as actual boxes rather than 4 rays.
        //      https://tavianator.com/2011/ray_box.html
        // Split map up into grid:
        //      https://gamedev.stackexchange.com/questions/18261/how-can-i-implement-fast-accurate-2d-collision-detection
        //      https://gamedev.stackexchange.com/questions/14369/how-could-you-parallelise-a-2d-boids-simulation
        // Part of both of these solutions might involve splitting things up into more groups, based on
        // static vs. moving (already split) and/or box vs ray colliders.
        if (move->velocity.x != 0) {
            position.y -= move->velocity.y;
            for (auto const& ray : foreignObstruction->rays) {
                Ray adjustedRay = addPointToRay(*ray, foreignObstruction->parentPos);
                if(ownObstruction->isColliding(adjustedRay, move->layer)) {
                    position.x -= move->velocity.x;
                    move->velocity.x = 0;
                    break;
                }
            }
            position.y += move->velocity.y;
        }
        if (move->velocity.y != 0) {
            position.x -= move->velocity.x;
            for (auto const& ray : foreignObstruction->rays) {
                Ray adjustedRay = addPointToRay(*ray, foreignObstruction->parentPos);
                if(ownObstruction->isColliding(adjustedRay, move->layer)) {
                    position.y -= move->velocity.y;
                    move->velocity.y = 0;
                    break;
                }
            }
            position.x += move->velocity.x;
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

void RealThing::AddToMap(map<string, RealThing*>& thingMap) {
    if (thingMap.count(name)) {
        cout << "TRIED TO ADD DUPLICATE NAMED ENTRY TO THING MAP OF SOME KIND" << endl;
        throw exception();
    }
    thingMap[name] = this;
}

Animator* RealThing::AddAnimator() {
    // Right now there is only one type of animator, but this could take
    // AnimationType in the future
    if (sprites.size() != 1) {
        std::cout << "Can't animate no sprites!" << std::endl;
        return nullptr;
    }
    animator = new Animator(sprites[0]);
    animator->splitSheet(9, 4); // Obviously this shouldn't be hard-coded, but for now it is
    bounds.top = 0 - sprites[0]->d.width;
    bounds.bottom = 0;
    bounds.left = 0 - ( sprites[0]->d.width / 2);
    bounds.right = (sprites[0]->d.width / 2);
    AddToMap(thingLists.animatedThings);
    return animator;
}

Move* RealThing::AddMove(MoveType type) {
    move = new Move(type, position);
    if (type == MoveType::automatic) {
        loadLuaFunc(sceneL, "beginAutoMove");
        lua_pushnumber(sceneL, move->origin.x);
        lua_pushnumber(sceneL, move->origin.y);
        lua_pushstring(sceneL, name.c_str());
        callLuaFunc(sceneL, 3, 0, 0);
    }
    AddToMap(thingLists.movinThings);
    return move;
}

void RealThing::AddStandardCollision() {
    vector<Ray> obstructionRays = {
        Ray(Point(bounds.right - 10, bounds.bottom), Point(bounds.left + 8, bounds.bottom)),
        Ray(Point(bounds.left + 8, bounds.bottom), Point(bounds.left + 8, bounds.bottom - 6)),
        Ray( Point(bounds.left + 8, bounds.bottom - 6), Point(bounds.right - 10, bounds.bottom - 6)),
        Ray(Point(bounds.right - 10, bounds.bottom - 6), Point(bounds.right - 10, bounds.bottom))
    };
    addInteractable("talk", obstructionRays, 0);
    addObstruction(obstructionRays, 0);
}

Sprite* RealThing::AddSprite(SpriteData SD) {
    sprites.push_back(new Sprite(position, SD));
    sprites.back()->active = true; // This was added to test lua loading, might have side effects
    calculateHeight();
    return sprites.back();
}

Sprite* RealThing::AddRawSprite(string textureName) {
    SpriteData sd;
    sd.textureName = textureName;
    return AddSprite(sd);
}

Interactable* RealThing::addInteractable(string iName, vector<Ray> rays, int layer) {
    if (interactables.count(iName))
        return interactables[iName];
    Interactable* in = new Interactable(position, name, iName, CollidableData(rays, layer));
    interactables[iName] = in;
    return in;
}

Trigger* RealThing::addTrigger(string iName, vector<Ray> rays, int layer) {
    if (triggers.count(iName))
        return triggers[iName];
    Trigger* tr = new Trigger(position, name, iName, CollidableData(rays, layer));
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

int RealThing::checkForCollidables(Ray incoming, int incomingLayer, CollidableType collidableType) {
    switch (collidableType) {
        case (CollidableType::interactable):
            for (auto const& [name, in] : interactables){
                if(in->isColliding(incoming, incomingLayer)) {
                    // Hit Lua to fire event
                    return 1;
                }
            }
            break;
        case (CollidableType::trigger):
            for (auto const& [name, tr] : triggers){
                if(tr->isColliding(incoming, incomingLayer)) {
                    // Hit Lua to fire event
                    return 0;
                }
            }
            break;
        case (CollidableType::obstruction):
            if(obstructions.count(incomingLayer) && obstructions[incomingLayer]->isColliding(incoming, incomingLayer))
                return 1;
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

void RealThing::highlightThing() {
    for (auto const& [id, s] : Sprite::sprites)
        s->alpha = 100;
    for (auto const& [id, s] : Sprite::sprites)
        s->alpha = 255;
}

void RealThing::removeHighlight() {
    for (auto s : sprites)
        s->alpha = 255;
}

RealThing* RealThing::copyInPlace() {
    return new RealThing(*this);
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
