#include "GameController.h"

GameController::GameController(lua_State *L) {
    this->L = L;
    controller = this;
}

bool GameController::meatEvent(KeyPresses keysDown) {
    vector<Task*> tasksToDelete;
    map<pair<Host*, string>, int> eventsToResume;
    bool blocking = false;
    for (int i = activeTasks.size() - 1; i >= 0; i--) {
        Task* t = activeTasks[i];
        pair<Host*, string> hostEventName = make_pair(t->host, t->eventName);
        if (t->meat(keysDown) < 1) {
            // task has exhausted subtasks
            if (!eventsToResume.count(hostEventName))
                eventsToResume[hostEventName] = eventArgKeys.at(hostEventName);
            tasksToDelete.push_back(t);
        } else {
            if (!eventsToResume.count(hostEventName))
                eventsToResume[hostEventName] = LUA_NOREF;
            else
                eventsToResume.at(hostEventName) = LUA_NOREF;
        }
        if (t->blocking) {
            blocking = true;
            break;
        }
    }
    for (auto t : tasksToDelete) {
        delete t;
        activeTasks.erase(remove(activeTasks.begin(), activeTasks.end(), t), activeTasks.end());
    }
    for (auto e : eventsToResume) {
        if (e.second == LUA_NOREF)
            continue;
        eventArgKeys.erase(e.first);
        // All tasks for this event have exhausted subtasks, so we look for more tasks
        loadLuaFunc("resumeEvent", e.first.first);
        lua_geti(L, LUA_REGISTRYINDEX, e.second);
        luaUtils::PushStringToTable(L, "eventName", e.first.second);
        callLuaFunc(1, 1, 0);
        luaL_unref(L, LUA_REGISTRYINDEX, e.second);
    }
    return blocking;
}

void GameController::meat(KeyPresses keysDown) {
    if (Scene::currentScene->sceneState == Scene::SceneState::pauseAll) {
        // Listen for unpause
        return;
    }
    Scene::currentScene->meat(keysDown, controller->activeTasks.size() > 0 && meatEvent(keysDown));
    performPendingSceneChange();
}

void GameController::requestSceneChange(Scene* newScene, RealThing* thing, Point destination, int newLayer) {
    pendingSceneChange.active = true;
    pendingSceneChange.newScene = newScene;
    pendingSceneChange.thing = thing;
    pendingSceneChange.destination = destination;
    pendingSceneChange.newLayer = newLayer;
}

void GameController::killAllTasks() {
    for (auto t : activeTasks)
        delete t;
    activeTasks.clear();
    for (auto const& [key, argKey] : eventArgKeys)
        luaL_unref(L, LUA_REGISTRYINDEX, argKey);
    eventArgKeys.clear();
    // Drop the Lua-side coroutines too, so nothing resumes against a deleted thing.
    loadLuaFunc("clearAllEvents");
    callLuaFunc(0, 0, 0);
}

void GameController::killEvent(Host* host, string eventName) {
    vector<Task*> survivors;
    for (auto t : activeTasks) {
        if (t->host == host && t->eventName == eventName)
            delete t;
        else
            survivors.push_back(t);
    }
    activeTasks = survivors;

    pair<Host*, string> key = {host, eventName};
    auto it = eventArgKeys.find(key);
    if (it != eventArgKeys.end()) {
        luaL_unref(L, LUA_REGISTRYINDEX, it->second);
        eventArgKeys.erase(it);
    }
    loadLuaFunc("clearEvent", host);
    lua_pushstring(L, eventName.c_str());
    callLuaFunc(1, 0, 0);
}

void GameController::performPendingSceneChange() {
    if (!pendingSceneChange.active)
        return;

    // 1. Kill all active tasks and clear Lua coroutines
    killAllTasks();

    Scene* oldScene = Scene::currentScene;
    Scene* newScene = pendingSceneChange.newScene;
    RealThing* player = pendingSceneChange.thing;

    // 2. Build the new scene's things.
    newScene->Load(false);

    // 3. Move the real player object to the new scene
    oldScene->things.erase(player->name);
    newScene->addExistingThingToScene(player);
    player->sceneThings = &newScene->things;

    // 4. Delete the old scene and enter the new one (player already moved out).
    newScene->EnterLoaded(player);
    player->position = pendingSceneChange.destination;
    player->shiftLayer(pendingSceneChange.newLayer);
    Camera::c->fadeIn(3);

    pendingSceneChange = PendingSceneChange();
}


int GameController::_newTask(lua_State *L) {
    if(!CheckParams(L, {ParamType::pointer, ParamType::str, ParamType::table })) {
        cout << "_newTask failed!" << endl;
        throw exception();
    }
    RealThing* hostThing = static_cast<RealThing*>(lua_touserdata(L, -1));
    lua_pop(L, 1);

    string eventName = lua_tostring(L, -1);
    pair<Host*, string> hostEventName = make_pair(hostThing, eventName);
    if (!controller->eventArgKeys.count(hostEventName)) {
        lua_newtable(hostThing->L);
        controller->eventArgKeys[hostEventName] = luaL_ref(hostThing->L, LUA_REGISTRYINDEX);
    }
    Task* newTask = new Task(eventName, hostThing, controller->eventArgKeys[hostEventName]);
    lua_pop(L, 1);

    lua_xmove(L, hostThing->L, 1);

    newTask->addSubtasks(hostThing->L);

    controller->activeTasks.push_back(newTask);
    lua_settop(L, 0);
    return 0;
}
