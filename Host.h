#include "luaUtils.h"

struct Host {
    lua_State* L;

    virtual void loadLuaFunc(std::string funcname) {
        lua_getglobal(L, funcname.c_str());
        if (!lua_isfunction(L, -1)) {
            std::cout << funcname << " is not function" << std::endl;
            throw std::exception();
        }
        lua_pushlightuserdata(L, this);
    }

    virtual void callLuaFunc(int nargs, int nresults, int errfunc) {
        if(!luaUtils::CheckLua(L, lua_pcall(L, nargs + 1, nresults, errfunc)))
            throw exception();
    }

    static bool GetHostPointerFromTable(lua_State *L, std::string key, Host*& hostPointer, int tableIndex = -1) {
        if (lua_istable(L, tableIndex)) {
            lua_pushstring(L, key.c_str());
            lua_gettable(L, tableIndex - 1);
            if (!lua_islightuserdata(L, -1)) {
                lua_pop(L, 1);
                return false;
            }
            hostPointer = static_cast<Host*>(lua_touserdata(L, -1));
            lua_pop(L, 1);
            return true;
        }
        std::cout << "PROBLEM : " << key << std::endl;
        return false;
    }
};