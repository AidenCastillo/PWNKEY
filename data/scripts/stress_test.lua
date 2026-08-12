print("=== Stress Test ===")

for i = 1, 10000 do
    local x = math.sin(i)
    if i % 1000 == 0 then
        print("Iteration:", i, "sin:", x)
    end
end

print("Done.")
