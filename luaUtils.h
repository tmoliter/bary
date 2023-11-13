#ifndef LUA_UTILS_H
#define LUA_UTILS_H
#include <string>
#include <iostream>

extern "C" {
    #include <lualib.h>
    #include <lauxlib.h>
}

namespace luaUtils {
    bool CheckLua(lua_State* L, int r);
    bool GetLuaStringFromTable(lua_State *L, std::string key, std::string &value, int tableIndex = -1);
    bool GetLuaIntFromTable(lua_State *L, std::string key, int &value, int tableIndex = -1);
    bool GetLuaBoolFromTable(lua_State *L, std::string key, bool &value, int tableIndex = -1);
    bool GetTableOnStackFromTable(lua_State *L, std::string key, int tableIndex = -1);

    bool PushStringToTable(lua_State* L , std::string key , std::string value, int tableIndex = -1);
    bool PushStringToTable(lua_State* L , int key , std::string value, int tableIndex = -1);
    bool PushIntToTable(lua_State* L , std::string key , int value, int tableIndex = -1);
    bool PushIntToTable(lua_State* L , int key , int value, int tableIndex = -1);
    bool PushTableToTable(lua_State* L , std::string key , int targetTableIndex = -1);
    bool PushTableToTable(lua_State* L , int key , int targetTableIndex = -1);

    void dumpstack (lua_State *L);
}
#endif
