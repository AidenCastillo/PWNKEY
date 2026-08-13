#include "engine/LuaScreen.h"
#include <lua.hpp>
#include <iostream>

// int screen_clear(lua_State* L);
// int screen_fill(lua_State* L);
// int screen_text(lua_State* L);
// int screen_center(lua_State* L);
// int screen_pixel(lua_State* L);
// int screen_rect(lua_State* L);

extern "C" int screen_clear(lua_State* L) {
    std::cout << "[NATIVE] Screen clear requested" << std::endl;
    return 0;
}

extern "C" int screen_fill(lua_State* L) {
    std::cout << "[NATIVE] Screen fill requested" << std::endl;
    return 0;
}

extern "C" int screen_text(lua_State* L) {
    const char* text = luaL_checkstring(L, 1);
    int x = luaL_checkinteger(L, 2);
    int y = luaL_checkinteger(L, 3);
    int fontSize = luaL_checkinteger(L, 4);
    int color = luaL_checkinteger(L, 5);

    std::cout << "[NATIVE] Screen text: \"" << text << "\" at (" << x << ", " << y
              << "), font size: " << fontSize << ", color: " << color << std::endl;

    return 0;
}

extern "C" int screen_center(lua_State* L) {
    const char* text = luaL_checkstring(L, 1);
    int x = luaL_checkinteger(L, 2);
    int y = luaL_checkinteger(L, 3);

    std::cout << "[NATIVE] Screen center: \"" << text << "\" at (" << x << ", " << y << ")" << std::endl;

    return 0;
}

extern "C" int screen_pixel(lua_State* L) {
    int x = luaL_checkinteger(L, 1);
    int y = luaL_checkinteger(L, 2);
    uint32_t color = luaL_checkinteger(L, 3);

    std::cout << "[NATIVE] Screen pixel at (" << x << ", " << y << ") with color: " << color << std::endl;

    return 0;
}

extern "C" int screen_rect(lua_State* L) {
    int x = luaL_checkinteger(L, 1);
    int y = luaL_checkinteger(L, 2);
    int width = luaL_checkinteger(L, 3);
    int height = luaL_checkinteger(L, 4);
    uint32_t color = luaL_checkinteger(L, 5);

    std::cout << "[NATIVE] Screen rect at (" << x << ", " << y << ") with width: " << width
              << ", height: " << height << ", color: " << color << std::endl;

    return 0;
}
