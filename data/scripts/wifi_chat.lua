print("=== Chat Server ===")

local port = 6060
print("Chat server on", port)

while true do
    local msg = wifi.createSocketServer(port)
    print("Client:", msg)
end
