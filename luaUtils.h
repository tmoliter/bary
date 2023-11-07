#include <string>
#include <iostream>

extern "C" {
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
}

namespace luaUtils {
    bool CheckLua(lua_State* L, int r);
    bool GetLuaStringFromTable(lua_State *L, std::string key, std::string &value, int tableIndex = -1);
    bool GetLuaIntFromTable(lua_State *L, std::string key, int &value, int tableIndex = -1);
    bool PushTableFromTable(lua_State *L, std::string key, int tableIndex = -1);
    void dumpstack (lua_State *L);
}
