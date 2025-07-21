#ifndef SCRIPT_ENGINE_H
#define SCRIPT_ENGINE_H

#include <Arduino.h>
#include <SD.h>

class ScriptEngine {
public:
    /**
     * @brief Virtual destructor for proper cleanup
     */
    virtual ~ScriptEngine() {}
    
    /**
     * @brief Initialize the script engine
     * @return true if initialization was successful
     */
    virtual bool initialize() = 0;
    
    /**
     * @brief Load a script from the SD card
     * @param filePath The path to the script file on the SD card
     * @return true if the script was loaded successfully
     */
    virtual bool loadScript(const char* filePath) = 0;
    
    /**
     * @brief Execute the currently loaded script
     * @return true if execution was successful
     */
    virtual bool execute() = 0;
    
    /**
     * @brief Register a native function that can be called from scripts
     * @param name The name to use in scripts
     * @param function Pointer to the native function
     * @return true if registration was successful
     */
    virtual bool registerFunction(const char* name, void* function) = 0;
    
    /**
     * @brief Clean up resources when done with the script
     */
    virtual void cleanup() = 0;
    
    /**
     * @brief Get the last error message
     * @return String containing the last error
     */
    virtual String getLastError() = 0;
    
    /**
     * @brief Get the name of the script engine
     * @return String containing the engine name
     */
    virtual String getEngineName() = 0;
};

/**
 * @brief Create an instance of the configured script engine
 * @return Pointer to the created script engine
 */
ScriptEngine* createScriptEngine();

#endif // SCRIPT_ENGINE_H
