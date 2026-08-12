print("=== Remote Command Server ===")

local port = 9090
print("Listening on port", port)

while true do
    local cmd = wifi.createSocketServer(port)
    print("CMD:", cmd)

    local fn, err = load(cmd)
    if not fn then
        print("Error:", err)
    else
        local ok, result = pcall(fn)
        print("Result:", result)
    end
end
