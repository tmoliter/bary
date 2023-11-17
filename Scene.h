#ifndef SCENE_H
#define SCENE_H
#include "FocusTracker.h"

using namespace luaUtils;

struct Scene {
    Scene(string name);

    inline static Scene* currentScene;

    lua_State *L;

    string name;

    map<string, RealThing*> things;
    map<string, RealThing*> movinThings;
    map<string, RealThing*> animatedThings;
    vector<string> thingsToDestroy;

    void meatThings(KeyPresses keysDown);

    RealThing* addThing(RealThingData tD);
    RealThing* addThing(Point p, string name = "AnonymousThing");
    RealThing* addThingToScene(RealThing* existingThing);

    RealThing* copyThing(RealThing& oldThing);

    void destroyThing(RealThing* thing);
    void destroyThings();
    void addThingToDestroyQueue(string n);
    void destroyAllThings();

    string renameThing(RealThing* thing, string newName);

    vector<RealThing*> findThingsByPoint(Point p);
    void showAllLines();
    void hideAllLines();

    int checkAllInteractables (Ray incoming, int incomingLayer);
    int checkAllTriggers (Ray incoming, int incomingLayer);

    RealThing *findRealThing(string name);

    void buildThingFromGlobal(lua_State* L);

    vector<RealThingData> getAllThingData();

    private:
        string getNewThingName(string name);
};

#endif