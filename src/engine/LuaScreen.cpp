#include "engine/LuaScreen.h"
#include "main.h"

static int fill(lua_State* L) {
    int color = luaL_checkinteger(L, 1);
    tft.fillScreen(color);
    return 0;
}

static int text(lua_State* L) {
    int x = luaL_checkinteger(L, 1);
    int y = luaL_checkinteger(L, 2);
    const char* msg = luaL_checkstring(L, 3);
    tft.drawString(msg, x, y);
    return 0;
}

static int center(lua_State* L) {
    const char* msg = luaL_checkstring(L, 1);
    int y = luaL_checkinteger(L, 2);
    tft.drawCentreString(msg, SCREEN_WIDTH / 2, y, FONT_SIZE);
    return 0;
}

void LuaScreen::registerModule(lua_State* L) {
    lua_newtable(L);

    lua_pushcfunction(L, fill);
    lua_setfield(L, -2, "fill");

    lua_pushcfunction(L, text);
    lua_setfield(L, -2, "text");

    lua_pushcfunction(L, center);
    lua_setfield(L, -2, "center");

    lua_setglobal(L, "screen");
}
