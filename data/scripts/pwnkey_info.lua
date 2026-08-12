print("=== PWNKEY Info ===")

print("Lua version:", _VERSION)
print("Engine:", SCRIPT_ENGINE_LUA)
print("Platform:", os.getenv("PLATFORM") or "unknown")
print("Time:", os.time())
