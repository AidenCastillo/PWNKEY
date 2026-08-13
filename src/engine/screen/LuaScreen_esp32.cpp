#include "engine/LuaScreen.h"
#include "main.h"
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>

// Declare the global Sprite pointer (allocated in main)
extern TFT_eSprite spr; 

extern "C" {
    int screen_clear(lua_State* L) {
        // Clear the off-screen buffer, not the physical screen
        spr.fillSprite(TFT_WHITE);
        return 0;
    }

    int screen_fill(lua_State* L) {
        // Read color from Lua if provided, default to WHITE
        uint32_t color = (lua_gettop(L) >= 1) ? luaL_checkinteger(L, 1) : TFT_WHITE;
        spr.fillSprite(color);
        return 0;
    }

    int screen_text(lua_State* L) {
        // Lua usage: screen.text("text", x, y, size, color)
        const char* text = luaL_checkstring(L, 1);
        int x = luaL_checkinteger(L, 2);
        int y = luaL_checkinteger(L, 3);
        int fontSize = luaL_checkinteger(L, 4);
        uint32_t color = luaL_checkinteger(L, 5);

        spr.setTextColor(color); // Set text color dynamically
        spr.drawString(text, x, y, fontSize);
        return 0;
    }

    int screen_center(lua_State* L) {
        // Lua usage: screen.center("text", x, y, size, color)
        const char* text = luaL_checkstring(L, 1);
        int x = luaL_checkinteger(L, 2);
        int y = luaL_checkinteger(L, 3);
        int fontSize = (lua_gettop(L) >= 4) ? luaL_checkinteger(L, 4) : FONT_SIZE;
        uint32_t color = (lua_gettop(L) >= 5) ? luaL_checkinteger(L, 5) : TFT_BLACK;

        spr.setTextColor(color);
        spr.drawCentreString(text, x, y, fontSize);
        return 0;
    }

    int screen_pixel(lua_State* L) {
        // Lua usage: screen.pixel(x, y, color)
        int x = luaL_checkinteger(L, 1);
        int y = luaL_checkinteger(L, 2);
        uint32_t color = luaL_checkinteger(L, 3);

        spr.drawPixel(x, y, color);
        return 0;
    }

    int screen_rect(lua_State* L) {
        // Lua usage: screen.rect(x, y, width, height, color)
        int x = luaL_checkinteger(L, 1);
        int y = luaL_checkinteger(L, 2);
        int width = luaL_checkinteger(L, 3);
        int height = luaL_checkinteger(L, 4);
        uint32_t color = luaL_checkinteger(L, 5);

        spr.fillRect(x, y, width, height, color);
        return 0;
    }

    // CRITICAL NEW FUNCTION: Pushes RAM buffer to the physical screen
    int screen_update(lua_State* L) {
        // Push the entire composed RAM sprite to physical coordinate (0,0)
        spr.pushSprite(0, 0); 
        
        // Feed FreeRTOS watchdog / allow CPU to rest (~60FPS limit)
        #ifdef ESP32
        vTaskDelay(pdMS_TO_TICKS(16)); 
        #endif
        
        return 0;
    }
}
