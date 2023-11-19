#include "luaUtils.h"

bool luaUtils::CheckLua(lua_State* L, int r) {
    if (r == LUA_OK) {
        return true;
    }
    std::string errmsg = lua_tostring(L, -1);
    std::cout << errmsg << std::endl;
    return false;
}

bool luaUtils::CheckParams(lua_State* L, std::vector<ParamType> params) {
    if (lua_gettop(L) != params.size()) {
        std::cout << "param length is not a correct!" << std::endl;
        return false;
    }
    int i = -1;
    for (auto p : params) {
      if (p == ParamType::str && !lua_isstring(L, i)) {
        std::cout << "param " << std::to_string(i) << " is not a string!" << std::endl;
        return false;
      }
      if (p == ParamType::number && !lua_isnumber(L, i)) {
        std::cout << "param " << std::to_string(i) << " is not a number!" << std::endl;
        return false;
      }
      if (p == ParamType::point && !lua_islightuserdata(L, i)) {
        std::cout << "param " << std::to_string(i) << " is not a pointer!" << std::endl;
        return false;
      }
      if (p == ParamType::table && !lua_istable(L, i)) {
        std::cout << "param " << std::to_string(i) << " is not a table!" << std::endl;
        return false;
      }
      i--;
    }
    return true;
}

bool luaUtils::GetLuaStringFromTable(lua_State *L, std::string key, std::string &value, int tableIndex) {
    if (lua_istable(L, tableIndex)) {
        lua_pushstring(L, key.c_str());
        lua_gettable(L, tableIndex - 1);
        if (!lua_isstring(L, -1)) {
            std::cout << key << " is not a string" << std::endl;
            return false;
        }
        value = lua_tostring(L, -1);
        lua_pop(L, 1);
        return true;
    }
    std::cout << "PROBLEM : " << key << std::endl;
    return false;
}

bool luaUtils::GetLuaIntFromTable(lua_State *L, std::string key, int &value, int tableIndex) {
    if (lua_istable(L, tableIndex)) {
        lua_pushstring(L, key.c_str());
        lua_gettable(L, tableIndex - 1);
        if (!lua_isnumber(L, -1)) {
            std::cout << key << " is not a number" << std::endl;
            return false;
        }
        value = lua_tointeger(L, -1);
        lua_pop(L, 1);
        return true;
    }
    std::cout << "PROBLEM : " << key << std::endl;
    return false;
}

bool luaUtils::GetLuaBoolFromTable(lua_State *L, std::string key, bool &value, int tableIndex) {
    if (lua_istable(L, tableIndex)) {
        lua_pushstring(L, key.c_str());
        lua_gettable(L, tableIndex - 1);
        if (!lua_isboolean(L, -1)) {
            std::cout << key << " is not a bool" << std::endl;
            return false;
        }
        value = lua_toboolean(L, -1);
        lua_pop(L, 1);
        return true;
    }
    std::cout << "PROBLEM : " << key << std::endl;
    return false;
}

bool luaUtils::GetLuaFuncOnStackFromTable(lua_State *L, std::string key, int tableIndex) {
    if (lua_istable(L, tableIndex)) {
        lua_pushstring(L, key.c_str());
        lua_gettable(L, tableIndex - 1);
        if (!lua_isfunction(L, -1)) {
            std::cout << key << " is not a func" << std::endl;
            return false;
        }
        return true;
    }
    std::cout << "PROBLEM : " << key << std::endl;
    return false;
}


bool luaUtils::GetTableOnStackFromTable(lua_State *L, std::string key, int tableIndex) {
    if (lua_istable(L, tableIndex)) {
        lua_pushstring(L, key.c_str());
        lua_gettable(L, tableIndex - 1);
        if (!lua_istable(L, -1)) {
            std::cout << key << " is not a table" << std::endl;
            return false;
        }
        return true;
    }
    std::cout << "PROBLEM : " << key << std::endl;
    return false;
}


bool luaUtils::PushStringToTable(lua_State* L , std::string key , std::string value, int tableIndex) {
    if (!lua_istable(L, tableIndex)) {
      std::cout << "PROBLEM : " << key << std::endl;
      return false;
    }
    lua_pushstring(L, key.c_str());
    lua_pushstring(L, value.c_str());
    lua_settable(L, tableIndex - 2);
    return true;
}

bool luaUtils::PushStringToTable(lua_State* L , int key , std::string value, int tableIndex) {
    if (!lua_istable(L, tableIndex)) {
      std::cout << "PROBLEM : " << key << std::endl;
      return false;
    }
    lua_pushinteger(L, key);
    lua_pushstring(L, value.c_str());
    lua_settable(L, tableIndex - 2);
    return true;
}

bool luaUtils::PushIntToTable(lua_State* L , std::string key , int value, int tableIndex) {
    if (!lua_istable(L, tableIndex)) {
      std::cout << "PROBLEM : " << key << std::endl;
      return false;
    }
    lua_pushstring(L, key.c_str());
    lua_pushinteger(L, value);
    lua_settable(L, tableIndex - 2);
    return true;
}

bool luaUtils::PushIntToTable(lua_State* L , int key , int value, int tableIndex) {
    if (!lua_istable(L, tableIndex)) {
      std::cout << "PROBLEM : " << key << std::endl;
      return false;
    }
    lua_pushinteger(L, key);
    lua_pushinteger(L, value);
    lua_settable(L, tableIndex - 2);
    return true;
}

bool luaUtils::PushTableToTable(lua_State* L , std::string key , int tableIndex) {
    if (!lua_istable(L, tableIndex)) {
      std::cout << "target table not table" << std::endl;
      return false;
    }
    lua_pushstring(L, key.c_str());
    lua_newtable(L);
    lua_settable(L, tableIndex - 2);
    lua_pushstring(L, key.c_str());
    lua_gettable(L, tableIndex - 1);
    return true;
}

bool luaUtils::PushTableToTable(lua_State* L , int key , int tableIndex) {
    if (!lua_istable(L, tableIndex)) {
      std::cout << "target table not table" << std::endl;
      return false;
    }
    lua_pushinteger(L, key);
    lua_newtable(L);
    lua_settable(L, tableIndex - 2);
    lua_pushinteger(L, key);
    lua_gettable(L, tableIndex - 1);
    return true;
}

// Slightly modified to show negative indices from post:
// https://stackoverflow.com/questions/59091462/from-c-how-can-i-print-the-contents-of-the-lua-stack
void luaUtils::dumpstack (lua_State *L) {
  int top=lua_gettop(L);
  for (int i=1; i <= top; i++) {
    printf("%d\t%s\t", i - top - 1, luaL_typename(L,i));
    switch (lua_type(L, i)) {
      case LUA_TNUMBER:
        printf("%g\n",lua_tonumber(L,i));
        break;
      case LUA_TSTRING:
        printf("%s\n",lua_tostring(L,i));
        break;
      case LUA_TBOOLEAN:
        printf("%s\n", (lua_toboolean(L, i) ? "true" : "false"));
        break;
      case LUA_TNIL:
        printf("%s\n", "nil");
        break;
      default:
        printf("%p\n",lua_topointer(L,i));
        break;
    }
  }
}
