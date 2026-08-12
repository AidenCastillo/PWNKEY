print("=== Echo Server ===")

local port = 7070
print("Listening on", port)

while true do
    local msg = wifi.createSocketServer(port)
    print("Client:", msg)
end
