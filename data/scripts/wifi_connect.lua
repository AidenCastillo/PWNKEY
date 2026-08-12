print("=== WiFi Connect Test ===")

local ssid = "YourSSID"
local pass = "YourPassword"

print("Connecting to:", ssid)
wifi.connect(ssid, pass)

local status = wifi.status()
print("WiFi status:", status)
