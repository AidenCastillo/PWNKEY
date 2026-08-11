#ifndef EngineBindings_h
#define EngineBindings_h
#include <lua.hpp>

#include "engine_bindings.h"
#ifdef ESP32
    #include <Arduino.h>
#else
    #include <iostream> // Needed for std::cout on desktop/native
#endif
// #include "gpio.h"
// #include "display.h"

// Example: digitalWrite(pin, value)
int lua_gpio_write(lua_State* L) {
    int pin = luaL_checkinteger(L, 1);
    int value = luaL_checkinteger(L, 2);

#ifdef ESP32
    // digitalWrite(pin, value);
    Serial.printf("[ESP32] GPIO write: %d = %d\n", pin, value);
#else
    std::cout << "[NATIVE] GPIO write: " << pin << " = " << value << std::endl;
#endif

    return 0; // number of return values
}

// Example: display.print("Hello")
int lua_display_print(lua_State* L) {
    const char* text = luaL_checkstring(L, 1);

#ifdef ESP32
    // display.print(text);
#else
    std::cout << "[NATIVE] Display: " << text << std::endl;
#endif

    return 0;
}

void register_engine_bindings(lua_State* L) {
    lua_register(L, "gpio_write", lua_gpio_write);
    lua_register(L, "display_print", lua_display_print);
}




#endif
