#ifndef LuaScriptEngine_h
#define LuaScriptEngine_h

#include "fs_interface.h"

#include <lua.hpp>

extern FSInterface* fs;

class LuaEngine {
    lua_State* L; // Pointer to the Lua state
public:
    LuaEngine() {
        L = luaL_newstate();
        luaL_openlibs(L);
    }
    ~LuaEngine() {
        lua_close(L);
    }

    bool runScript(const std::string& code) {
        if (luaL_loadstring(L, code.c_str()) || lua_pcall(L, 0, 0, 0)) {
            printf("Lua Error: %s\n", lua_tostring(L, -1));
            return false;
        }
        return true;
    }
    
    bool runFile(const std::string& path) {
        auto file = fs->open(path);
        if (file.content.empty()) {
            printf("LuaEngine: File not found: %s\n", path.c_str());
            return false;
        }
        return runScript(file.content);
    }
    lua_State* getState() { return L; }
};

#endif
