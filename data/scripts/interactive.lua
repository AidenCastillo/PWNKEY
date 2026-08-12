print("=== Interactive Lua ===")

while true do
    io.write("> ")
    local line = io.read("*l")

    local fn, err = load(line)
    if not fn then
        print("Error:", err)
    else
        local ok, result = pcall(fn)
        if not ok then
            print("Error:", result)
        else
            print("Result:", result)
        end
    end
end
