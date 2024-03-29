#ifndef VISIBLE_THING_H
#define VISIBLE_THING_H

#include "components/Obstruction.h"
#include "components/EventCollidable.h"
#include "components/Animator.h"
#include "components/Move.h"
#include "Host.h"

using namespace std;

enum class ThingType {
    fieldPlayer,
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
    string name;
    ThingType type;
    bool isSub = false;

    map<string, RealThing*>& things;
    Host* parentScene;

    Point position;
    Point origin;
    Bounds bounds;

    vector<RealThing*> subThings;
    vector<RealThing*> getSelfAndSubs();

    Point getCenter();
    string getBaseName();

    void manuallyControl(KeyPresses keysDown);

    virtual void processMove(KeyPresses keysDown);
    virtual void processCollisions(map<string, RealThing*>& things);
    virtual void animate(KeyPresses keysDown);
    virtual void meat(KeyPresses keysDown);

    RealThing(RealThingData tD, map<string, RealThing*>& tL);
    RealThing(RealThing &oldThing);
    ~RealThing();

    vector<Sprite*> sprites;
    Animator* animator;
    Move* move;

    map<int, Obstruction*> obstructions;
    map<string, Interactable*> interactables;
    map<string, Trigger*> triggers;

    void calculateHeight();

    void AddToMap(map<string, RealThing*>& thingMap);

    void addComponentsFromTable();
    Animator* AddAnimator();
    Move* AddMove(MoveType type);
    void AddStandardCollision(vector<CollidableType> eventCollidables = {}, vector<string> eventNames = {});

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

    void shiftLayer(int newLayer);

    virtual int checkForCollidables(Ray incoming, int incomingLayer, RealThing* incomingThing, CollidableType collidableType);

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


    void PushThingDataOnStack();
    RealThingData getData();

    virtual RealThing* copyInPlace();

    static int _getThingData(lua_State* L); // currently unused
};

#endif
