#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>
#include <iostream>
#include "fs_native.h"
#include "loader.h"

TEST_CASE("LuaEngine runScript") {
    NativeFS native;
    fs = &native;

    LuaEngine lua;
    CHECK(lua.runScript("x = 42") == true);
    CHECK(lua.runScript("print(x)") == true);
    CHECK(lua.runScript("error('test error')") == false);
}

TEST_CASE("LuaEngine runFile") {
    NativeFS native;
    fs = &native;

    native.mkdir("build");
    native.mkdir("build/native_tests");
    native.mkdir("build/native_tests/test");
    native.mkdir("build/native_tests/test/scripts");
    
    std::ofstream out("build/native_tests/test/scripts/test_file.lua");
    out << "y = 100";
    out.close();

    std::cout << "Created test_file.lua" << std::endl;

    std::cout << "Running test_file.lua" << std::endl;

    LuaEngine lua;
    CHECK(lua.runFile("build/native_tests/test/scripts/test_file.lua") == true);
    CHECK(lua.runScript("print(y)") == true);
    CHECK(lua.runFile("build/native_tests/test/scripts/nonexistent.lua") == false);
}

TEST_CASE("Run prewritten Lua script from data/scripts/test_lua.lua") {
    // Use native filesystem
    NativeFS native;
    fs = &native;

    CHECK(fs_init() == true);

    // Run the Lua script
    LuaEngine lua;
    bool ok = lua.runFile("data/scripts/test_lua.lua");

    CHECK(ok == true);
}

int run_doctest_tests() {
    doctest::Context ctx;
    // ctx.applyCommandLine();
    return ctx.run();
}
