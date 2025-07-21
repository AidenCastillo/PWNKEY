#ifndef CUSTOM_SCRIPT_ENGINE_H
#define CUSTOM_SCRIPT_ENGINE_H

#include "script_engine.h"

/**
 * @brief Custom simple script engine implementation
 * 
 * This engine implements a simple command-based scripting language
 * for PWNKEY modules.
 */
class CustomScriptEngine : public ScriptEngine {
private:
    File scriptFile;
    char lineBuffer[256];
    String lastError;
    
    // Function pointer type for command handlers
    typedef bool (*CommandHandler)(const char* params);
    
    // Structure to map command names to handlers
    struct CommandMapping {
        const char* command;
        CommandHandler handler;
    };
    
    // Array of command mappings
    static CommandMapping commandMappings[32];
    static int numCommands;
    
    // Parse a command line and execute the appropriate handler
    bool executeCommand(const char* cmd);
    
    // Helper functions to extract parameters
    static int extractIntParam(const char* cmd, const char* paramName);
    static bool extractBoolParam(const char* cmd, const char* paramName);
    static const char* extractStringParam(const char* cmd, const char* paramName);

public:
    CustomScriptEngine();
    virtual ~CustomScriptEngine();
    
    bool initialize() override;
    bool loadScript(const char* filePath) override;
    bool execute() override;
    bool registerFunction(const char* name, void* function) override;
    void cleanup() override;
    String getLastError() override;
    String getEngineName() override { return "CustomScriptEngine"; }
};

#endif // CUSTOM_SCRIPT_ENGINE_H
