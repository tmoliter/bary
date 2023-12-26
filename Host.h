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
};