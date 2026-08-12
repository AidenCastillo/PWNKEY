#include <lua.hpp>
#include "engine/LuaScreen.h"

/*
    Shared Lua wrapper for Screen module.
    Backend implementations live in:
        - LuaScreen_esp32.cpp
        - LuaScreen_native.cpp

    These backend files define:
        static int fill(lua_State* L);
        static int text(lua_State* L);
        static int center(lua_State* L);
        static int pixel(lua_State* L);
        static int rect(lua_State* L);

    This file ONLY registers Lua functions.
*/

// Backend function declarations (implemented in platform-specific files)
extern "C" {
    int screen_clear(lua_State* L);
    int screen_fill(lua_State* L);
    int screen_text(lua_State* L);
    int screen_center(lua_State* L);
    int screen_pixel(lua_State* L);
    int screen_rect(lua_State* L);
}

void LuaScreen::registerModule(lua_State* L) {
    lua_newtable(L);

    // screen.clear()
    lua_pushcfunction(L, screen_clear);
    lua_setfield(L, -2, "clear");

    // screen.fill()
    lua_pushcfunction(L, screen_fill);
    lua_setfield(L, -2, "fill");

    // screen.text(x, y, text)
    lua_pushcfunction(L, screen_text);
    lua_setfield(L, -2, "text");

    // screen.center(x, y)
    lua_pushcfunction(L, screen_center);
    lua_setfield(L, -2, "center");

    // screen.pixel(x, y)
    lua_pushcfunction(L, screen_pixel);
    lua_setfield(L, -2, "pixel");

    // screen.rect(x, y, width, height)
    lua_pushcfunction(L, screen_rect);
    lua_setfield(L, -2, "rect");

    // Expose table as global "screen"
    lua_setglobal(L, "screen");
}
