#ifndef VISIBLE_THING_H
#define VISIBLE_THING_H

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include "components/Obstruction.h"
#include "components/EventCollidable.h"
#include "components/Animator.h"
#include "components/Move.h"
#include "Host.h"

// dubiously from Thing.h
#include <string>

using namespace std;

enum class ThingType {
    fieldPlayer,
    door,
    npc,
    thing,
};

struct RealThingData {
    RealThingData() {};
    RealThingData(Point p, string n = "AnonymousThing", string textureName = "") : x(p.x), y(p.y), name(n) {
        if (textureName != "")
            spriteDataVector.push_back(SpriteData(textureName));
    };
    string name = "AnonymousThing";
    int x = 0;
    int y = 0;

    vector<SpriteData> spriteDataVector;
    vector<CollidableData> obstructionData;
};

struct RealThing : public Host {
    struct ThingLists {
        ThingLists(
            map<string, RealThing*>& things
        ) :
        things(things) {}

        map<string, RealThing*>& things;
    };

    string name;
    ThingType type;

    ThingLists thingLists;
    Host* parentScene;

    Point position;
    Bounds bounds;

    vector<RealThing*> subThings;


    Point getCenter();

    void manuallyControl(KeyPresses keysDown);

    virtual void processMove(KeyPresses keysDown);
    virtual void processCollisions(map<string, RealThing*>& things);
    virtual void animate(KeyPresses keysDown);
    virtual void meat(KeyPresses keysDown);

    RealThing(RealThingData tD, ThingLists tL);
    RealThing(RealThing &oldThing);
    ~RealThing();

    vector<Sprite*> sprites;
    Animator* animator;
    Move* move;

    map<int, Obstruction*> obstructions;
    map<string, Interactable*> interactables;
    map<string, Trigger*> triggers;

    void loadLuaFunc(std::string funcname);
    void callLuaFunc(int nargs, int nresults, int errfunc);

    void calculateHeight();

    void AddToMap(map<string, RealThing*>& thingMap);

    void addComponentsFromTable();
    Animator* AddAnimator();
    Move* AddMove(MoveType type);
    void AddStandardCollision();

    Sprite* AddSprite(SpriteData SD);
    Sprite* AddRawSprite(string path);

    Interactable* addInteractable(string iName, vector<Ray> rays, int layer);
    Trigger* addTrigger(string iName, vector<Ray> rays, int layer);
    Obstruction* addObstruction(vector<Ray> rays, int layer);
    Interactable* addInteractable(string iName);
    Trigger* addTrigger(string iName);
    Obstruction* addObstruction(int layer);

    void RemoveSprite(Sprite* sprite);
    void removeObstruction(int layer);
    void removeInteractable(string name);
    void removeTrigger(string name);

    void removeAllCollidables();

    virtual int checkForCollidables(Ray incoming, int incomingLayer, CollidableType collidableType);

    void showObstructionLines(int layer = -1001);
    void showInteractableLines(int layer = -1001, string name = "");
    void showTriggerLines(int layer = -1001, string name = "");
    void showLines();
    void hideObstructionLines();
    void hideInteractableLines();
    void hideTriggerLines();
    void hideLines();

    void highlightThing();
    void highlightSprite(Sprite* sprite);
    void removeHighlight();

    vector<string> findAndShowInteractableLines(string beginning);
    vector<string> findAndShowTriggerLines(string beginning);



    RealThingData getData();

    virtual RealThing* copyInPlace();
};

#endif
