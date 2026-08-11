#ifndef LuaScriptEngine_h
#define LuaScriptEngine_h

#include "fs_interface.h"
#include "engine_bindings.h"
#include "engine/LuaWifi.h"
#include "module.h"

#include <lua.hpp>

extern FSInterface* fileSystemInterface;

class LuaModule;

class LuaEngine {
    lua_State* L;
public:
    LuaEngine() {
        L = luaL_newstate();
        luaL_openlibs(L);

        // Register your custom functions
        register_engine_bindings(L);
        LuaWifi::registerModule(L);
    }

    ~LuaEngine() {
        lua_close(L);
    }

    LuaModule* createModule(const std::string& name, const std::string& script);

    bool runScript(const std::string& code) {
        if (luaL_loadstring(L, code.c_str()) || lua_pcall(L, 0, 0, 0)) {
            printf("Lua Error: %s\n", lua_tostring(L, -1));
            return false;
        }
        return true;
    }
    
    bool runFile(const std::string& path) {
        auto file = fileSystemInterface->open(path);
        if (file.content.empty()) {
            printf("LuaEngine: File not found: %s\n", path.c_str());
            return false;
        }
        return runScript(file.content);
    }
    lua_State* getState() { return L; }
};

class LuaModule : public Module {
    std::string moduleName;
    std::string luaScript;
public:
    LuaModule(const std::string& name, const std::string& script)
        : moduleName(name), luaScript(script) {}

    std::string getName() const override {
        return moduleName;
    }

    void init() override {
    }

    std::string getInfo() const override {
        return "Lua Module: " + moduleName;
    }

    void run() override {
        // Run the Lua script
        LuaEngine lua;
        lua.runScript(luaScript);
    }

    void cleanup() override {
        // Cleanup if needed
    }
};

inline LuaModule* LuaEngine::createModule(const std::string& name, const std::string& script) {
    return new LuaModule(name, script);
}

#endif
