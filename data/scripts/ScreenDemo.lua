
while (true) do
    screen.clear()

    screen.fill(0xFFFF)

    screen.text("Hello, LuaScreen!", 10, 10, 2, 0x0000)

    -- screen.center("Centered Text", 0xF800)

    screen.pixel(50, 50, 0x07E0)

    screen.rect(20, 20, 100, 50, 0x001F)

    -- screen.update()

    -- os.sleep(1)
end
