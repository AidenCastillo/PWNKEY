print("=== GPIO Test ===")

for i = 1, 10 do
    print("Setting GPIO 5 HIGH")
    gpio.write(5, 1)
    os.sleep(500)

    print("Setting GPIO 5 LOW")
    gpio.write(5, 0)
    os.sleep(500)
end
