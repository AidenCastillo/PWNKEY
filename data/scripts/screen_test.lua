print("=== Screen Test ===")

screen.clear()
-- screen.print("Hello from Lua!")
-- screen.print("Time: " .. os.time())
while (true) do
    screen.rect(10, 10, 100, 50, 0xFFFF)
    -- screen.line(10, 10, 110, 60, 0xF800)
    -- screen.circle(60, 35, 20, 0x07E0)

    screen.pixel(60, 35, 0x001F)
    screen.pixel(61, 35, 0x001F)

    -- screen.update()
end
