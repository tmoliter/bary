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
        // Class Members PORTED FROM Thing.h
        Point position;

        Bounds bounds;
        string name;
        vector<RealThing*> subThings;

        string rename(string newName);
        Point getCenter();

        void manuallyControl(KeyPresses keysDown);

        virtual void destroy();

        virtual void processMove(KeyPresses keysDown);
        virtual void processCollisions();
        virtual void animate(KeyPresses keysDown);
        virtual void meat(KeyPresses keysDown);

        // Static Members PORTED FROM Thing.h

        inline static map<string, RealThing*> things;
        inline static map<string, RealThing*> movinThings;
        inline static map<string, RealThing*> animatedThings;
        inline static vector<string> thingsToDestroy;

        static void meatThings(KeyPresses keysDown);
        static void destroyThings();
        static void destroyThing(string n);
        static void destroyAllThings();

        static vector<RealThing*> findThingsByPoint(Point p);

        // EXISTED BEFORE
        RealThing(RealThingData tD);
        RealThing(Point p, string name);
        RealThing(Point p);
        RealThing(RealThing &oldThing);
        ~RealThing();

        void _save_name_and_save_in_map(string n);

        vector<Sprite*> sprites;
        Animator* animator;
        Move* move;

        map<int, Obstruction*> obstructions;
        map<string, Interactable*> interactables;
        map<string, Trigger*> triggers;

        void calculateHeight();

        Animator* AddAnimator();
        Move* AddMove();

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

        vector<string> findAndShowInteractableLines(string beginning);
        vector<string> findAndShowTriggerLines(string beginning);

        void highlightSprite(Sprite* sprite);
        void removeHighlight();

        RealThingData getData();

        virtual RealThing* copyInPlace();

        static void showAllLines();
        static void hideAllLines();

        static int checkAllObstructions (Ray incoming, int incomingLayer);
        static int checkAllInteractables (Ray incoming, int incomingLayer);
        static int checkAllTriggers (Ray incoming, int incomingLayer);

        static RealThing *findRealThing(string name);

        static void buildThingFromGlobal(lua_State* L);
        static vector<RealThingData> getAllThingData();
};

#endif
