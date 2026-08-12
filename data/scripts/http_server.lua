print("=== HTTP Server ===")

local port = 80
print("Listening on port 80")

while true do
    local req = wifi.createSocketServer(port)
    print("Request:", req)

    local response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nHello from PWNKEY!"
    print("Sending response...")
    -- Your native backend will need to support sending data back
end
