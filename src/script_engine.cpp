#include "script_engine.h"

// Include necessary headers based on build configuration
#if defined(SCRIPT_ENGINE_CUSTOM) && SCRIPT_ENGINE_CUSTOM
#include "custom_script_engine.h"
#endif

#if defined(SCRIPT_ENGINE_LUA) && SCRIPT_ENGINE_LUA
#include "lua_script_engine.h"
#endif

/**
 * Factory function to create the appropriate script engine
 * based on compile-time configuration.
 */
ScriptEngine* createScriptEngine() {
    #if defined(SCRIPT_ENGINE_CUSTOM) && SCRIPT_ENGINE_CUSTOM
    return new CustomScriptEngine();
    #elif defined(SCRIPT_ENGINE_LUA) && SCRIPT_ENGINE_LUA
    return new LuaScriptEngine();
    #else
    #error "No script engine defined. Set SCRIPT_ENGINE_CUSTOM=1 or SCRIPT_ENGINE_LUA=1"
    #endif
}
