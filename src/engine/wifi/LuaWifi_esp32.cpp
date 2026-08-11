#include "engine/LuaWifi.h"
#include <WiFi.h>

extern "C" int wifi_connect(lua_State* L) {
    WiFi.begin(luaL_checkstring(L, 1), luaL_checkstring(L, 2));
    return 0;
}

extern "C" int wifi_status(lua_State* L) {
    lua_pushinteger(L, WiFi.status());
    return 1;
}

extern "C" int wifi_scan(lua_State* L) {
    int n = WiFi.scanNetworks();
    lua_newtable(L);
    for (int i = 0; i < n; i++) {
        lua_pushinteger(L, i + 1);
        lua_pushstring(L, WiFi.SSID(i).c_str());
        lua_settable(L, -3);
    }
    return 1;
}

extern "C" int wifi_createSocketServer(lua_State* L) {
    // ESP32 version later if needed
    return 0;
}
