#pragma once
#include <lua.hpp>

class LuaScreen {
public:
    static void registerModule(lua_State* L);
};
