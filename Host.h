#include "luaUtils.h"

struct Host {
    virtual void loadLuaFunc(lua_State *L, std::string funcname) {
        lua_getglobal(L, funcname.c_str());
        if (!lua_isfunction(L, -1)) {
            std::cout << funcname << " is not function" << std::endl;
            throw std::exception();
        }
        lua_pushlightuserdata(L, this);
    }

    virtual void callLuaFunc(lua_State *L, int nargs, int nresults, int errfunc) {
        if(!luaUtils::CheckLua(L, lua_pcall(L, nargs + 1, nresults, errfunc)))
            throw exception();
    }
};