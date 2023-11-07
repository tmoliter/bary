#include "MapParser.h"

using namespace std;

void parse_map() {
    MapBuilder *m = new MapBuilder();
    RealThing* focus = m->currentThing;
    new Camera(renderer);
    new FocusTracker();
    FocusTracker::ftracker->setFocus(focus);
    Camera::c->path = "./assets/backgrounds/Burg.png";
    Camera::c->init();

    lua_State* L = luaL_newstate();
    if (CheckLua(L, luaL_dofile(L, "storage/maps/burg.lua"))) {
        lua_getglobal(L, "allThings");
        if(!lua_isnil(L, -1)) {
            lua_pushnil(L);
            while (lua_next(L, -2)) {
                RealThing::buildThingFromGlobal(L);
            }
        }
    } else {
        string errmsg = lua_tostring(L, -1);
        cout << errmsg << endl; 
    }
    lua_close(L);
}