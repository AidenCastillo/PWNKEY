#include "engine/LuaWifi.h"
#include <lua.hpp>

/*
    Shared Lua wrapper for WiFi module.
    Backend implementations live in:
        - LuaWifi_esp32.cpp
        - LuaWifi_native.cpp

    These backend files define:
        int wifi_connect(lua_State* L);
        int wifi_status(lua_State* L);
        int wifi_scan(lua_State* L);
        int wifi_createSocketServer(lua_State* L);

    This file ONLY registers Lua functions.
*/

// Backend function declarations (implemented in platform-specific files)
extern "C" {
    int wifi_connect(lua_State* L);
    int wifi_status(lua_State* L);
    int wifi_scan(lua_State* L);
    int wifi_createSocketServer(lua_State* L);
}

void LuaWifi::registerModule(lua_State* L) {
    lua_newtable(L);

    // wifi.connect(ssid, pass)
    lua_pushcfunction(L, wifi_connect);
    lua_setfield(L, -2, "connect");

    // wifi.status()
    lua_pushcfunction(L, wifi_status);
    lua_setfield(L, -2, "status");

    // wifi.scan()
    lua_pushcfunction(L, wifi_scan);
    lua_setfield(L, -2, "scan");

    // wifi.createSocketServer(port)
    lua_pushcfunction(L, wifi_createSocketServer);
    lua_setfield(L, -2, "createSocketServer");

    // Expose table as global "wifi"
    lua_setglobal(L, "wifi");
}
