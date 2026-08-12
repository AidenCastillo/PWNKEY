print("=== WiFi Scan ===")

local networks = wifi.scan()

for i, name in pairs(networks) do
    print(i .. ": " .. name)
end
