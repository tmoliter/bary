#include "luaUtils.h"

struct Host {
    lua_State* L;

    virtual void loadLuaFunc(std::string funcname, Host* host = nullptr) {
        if (host == nullptr)
            host = this;
        lua_getglobal(L, funcname.c_str());
        if (!lua_isfunction(L, -1)) {
            std::cout << funcname << " is not function" << std::endl;
            throw std::exception();
        }
        lua_pushlightuserdata(L, host);
    }

    virtual void callLuaFunc(int nargs, int nresults, int errfunc) {
        if(!luaUtils::CheckLua(L, lua_pcall(L, nargs + 1, nresults, errfunc)))
            throw exception();
    }

    static bool PushHostToTable(lua_State* L , std::string key, Host* value, int tableIndex = -1) {
        if (!lua_istable(L, tableIndex)) {
            std::cout << "PROBLEM : " << key << std::endl;
            return false;
        }
        lua_pushstring(L, key.c_str());
        lua_pushlightuserdata(L, value);
        lua_settable(L, tableIndex - 2);
        return true;
    }

    static bool PushHostToTable(lua_State* L , int key , Host* value, int tableIndex = -1) {
        if (!lua_istable(L, tableIndex)) {
            std::cout << "PROBLEM : " << key << std::endl;
            return false;
        }
        lua_pushinteger(L, key);
        lua_pushlightuserdata(L, value);
        lua_settable(L, tableIndex - 2);
        return true;
    }

    static bool GetLuaHostFromTable(lua_State *L, std::string key, Host* &value, int tableIndex = -1) {
        if (lua_istable(L, tableIndex)) {
            lua_pushstring(L, key.c_str());
            lua_gettable(L, tableIndex - 1);
            if (!lua_islightuserdata(L, -1)) {
                lua_pop(L, 1);
                return false;
            }
            value = static_cast<Host*>(lua_touserdata(L, -1));
            lua_pop(L, 1);
            return true;
        }
        std::cout << "PROBLEM : " << key << std::endl;
        return false;
    }

};
