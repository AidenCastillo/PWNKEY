-- Test file to check if the WiFi module is working correctly
-- local wifi = require("wifi")

print("Testing WiFi module...")
print("Creating socket server on port 8080...")

local msg, err = wifi.createSocketServer(8080)
if not msg then
    print("Error:", err)
end
print("Client connected:", msg)

