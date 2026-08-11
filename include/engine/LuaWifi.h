#pragma once
#include <lua.hpp>

class LuaWifi {
public:
    static void registerModule(lua_State* L);
};
