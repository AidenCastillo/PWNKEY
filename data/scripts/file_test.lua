print("=== File Test ===")

local f = io.open("/sd/test.txt", "w")
f:write("Hello SD card!")
f:close()

local r = io.open("/sd/test.txt", "r")
print("Read:", r:read("*a"))
r:close()
