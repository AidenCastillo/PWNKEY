-- Example for testing Lua script engine
local function test()
    print("Hello from Lua!")
    return "Lua test passed"
end

local result = test()
print(result)

local function add(a, b)
    return a + b
end

local sum = add(5, 7)
print("Sum of 5 and 7 is: " .. sum)

print("Lua script running!")

gpio_write(5, 1)            -- turn on LED
display_print("Hello Lua!") -- draw text
