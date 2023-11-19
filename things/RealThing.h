#ifndef VISIBLE_THING_H
#define VISIBLE_THING_H

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include "components/Obstruction.h"
#include "components/Interactable.h"
#include "components/Animator.h"
#include "components/Move.h"

// dubiously from Thing.h
#include <string>

using namespace std;

struct RealThingData {
    // ported from ThingData
    string name;
    int x;
    int y;

    vector<SpriteData> spriteDataVector;
    vector<CollidableData> obstructionData;
};

// NEXT THING: COPY SIMPLE MESSAGES (or don't) AND DOORS

class RealThing {
    public:
        string name;

        Point position;
        Bounds bounds;

        lua_State* sceneL;

        vector<RealThing*> subThings;

        Point getCenter();

        void manuallyControl(KeyPresses keysDown);

        virtual void processMove(KeyPresses keysDown);
        virtual void processCollisions(map<string, RealThing*>& things);
        virtual void animate(KeyPresses keysDown);
        virtual void meat(KeyPresses keysDown);

        RealThing(RealThingData tD);
        RealThing(Point p, string name);
        RealThing(RealThing &oldThing);
        ~RealThing();

        vector<Sprite*> sprites;
        Animator* animator;
        Move* move;

        map<int, Obstruction*> obstructions;
        map<string, Interactable*> interactables;
        map<string, Trigger*> triggers;

        void calculateHeight();

        Sprite* AddSprite(SpriteData SD);
        Sprite* AddRawSprite(string path);

        Interactable* addInteractable(string iName, vector<Ray*> rays, int layer, Event* event = nullptr);
        Trigger* addTrigger(string iName, vector<Ray*> rays, int layer, Event* event = nullptr);
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
