#ifndef SCENE_H
#define SCENE_H
#include "FocusTracker.h"
#include "things/FieldPlayer.h"
#include "things/Door.h"

using namespace luaUtils;

struct Scene {
    inline static Scene* currentScene;

    lua_State *L;

    string sceneName;
    string backgroundPath;

    map<string, RealThing*> things;
    map<string, RealThing*> movinThings;
    map<string, RealThing*> animatedThings;
    vector<string> thingsToDestroy;

    Scene(string sceneName);
    ~Scene();

    void Load();
    void EnterLoaded(RealThing* focus);

    void meatThings(KeyPresses keysDown);

    RealThing* addThing(RealThingData tD, ThingType type = ThingType::thing);
    RealThing* addExistingThingToScene(RealThing* existingThing);

    RealThing* copyThing(RealThing& oldThing);

    void destroyThing(RealThing* thing);
    void destroyThings();
    void addThingToDestroyQueue(string n);
    void destroyAllThings();

    string renameThing(RealThing* thing, string newName);

    vector<RealThing*> findThingsByPoint(Point p);
    void showAllLines();
    void hideAllLines();

    RealThing *findRealThing(string name);

    RealThing* buildThingFromTable(lua_State* L);
    void addComponentsFromTable(lua_State* L, RealThing* thing);

    vector<RealThingData> getAllThingData();

    static int _loadScene(lua_State* L);
    static int _createThing(lua_State* L);
    static int _updateMoveTarget(lua_State *L);

    RealThing::ThingLists getThingLists();
    private:
        string getNewThingName(string name);
};

#endif