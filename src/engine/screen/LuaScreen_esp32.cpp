#include "engine/LuaScreen.h"
#include "main.h"
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>

extern "C" {
    int screen_clear(lua_State* L) {
        tft.fillScreen(TFT_WHITE);
        return 0;
    }
    int screen_fill(lua_State* L) {
        tft.fillScreen(TFT_WHITE);
        return 0;
    }
    int screen_text(lua_State* L) {
        // screen.text("Hello, LuaScreen!", 10, 10, 2, 0x0000)
        
        const char* text = luaL_checkstring(L, 1);
        int x = luaL_checkinteger(L, 2);
        int y = luaL_checkinteger(L, 3);
        int fontSize = luaL_checkinteger(L, 4);
        int color = luaL_checkinteger(L, 5);

        tft.setCursor(x, y);
        tft.setTextColor(TFT_BLACK, TFT_WHITE);
        tft.print(text);

        return 0;
    }
    int screen_center(lua_State* L) {
        // screen.center("Centered Text", 0xF800)
        const char* text = luaL_checkstring(L, 1);
        int x = luaL_checkinteger(L, 2);
        int y = luaL_checkinteger(L, 3);

        tft.setTextColor(TFT_BLACK, TFT_WHITE);
        tft.drawCentreString(text, x, y, FONT_SIZE);

        return 0;
    }
    int screen_pixel(lua_State* L) {
        int x = luaL_checkinteger(L, 1);
        int y = luaL_checkinteger(L, 2);
        uint32_t color = luaL_checkinteger(L, 3);

        tft.drawPixel(x, y, color);

        return 0;
    }
    int screen_rect(lua_State* L) {
        int x = luaL_checkinteger(L, 1);
        int y = luaL_checkinteger(L, 2);
        int width = luaL_checkinteger(L, 3);
        int height = luaL_checkinteger(L, 4);
        uint32_t color = luaL_checkinteger(L, 5);

        tft.fillRect(x, y, width, height, color);

        return 0;
    }
}
