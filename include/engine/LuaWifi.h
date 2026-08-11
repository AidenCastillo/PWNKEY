#pragma once
#include <lua.hpp>

class LuaWiFi {
public:
    static void registerModule(lua_State* L);
};
