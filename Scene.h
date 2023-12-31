#ifndef SCENE_H
#define SCENE_H
#include "FocusTracker.h"
#include "things/FieldPlayer.h"
#include "things/Door.h"
#include "Task.h"

using namespace luaUtils;

struct Scene : public Host {
    inline static Scene* currentScene;

    enum class SceneState {
        play,
        pausePlayerControl,
        pauseThings,
        pauseAll,
    } sceneState;

    vector<Task*> activeTasks;

    string sceneName;
    string backgroundPath;

    map<string, RealThing*> things;
    vector<string> thingsToDestroy;

    Scene(string sceneName);
    ~Scene();

    void Load();
    void EnterLoaded(RealThing* focus);

    void meat(KeyPresses keysDown);
    bool meatEvent(KeyPresses keysDown);
    void meatThings(KeyPresses keysDown, bool blockingEvent);

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

    RealThing* buildThingFromTable();

    vector<RealThingData> getAllThingData();

    static int _loadScene(lua_State* L);
    static int _createThing(lua_State* L);
    static int _newTask(lua_State *L);

    private:
        string getNewThingName(string name);
};

#endif