import os, tarfile, urllib.request, shutil

LUA_VERSION = "5.5.1"
URL = f"https://www.lua.org/ftp/lua-{LUA_VERSION}.tar.gz"
DEST = "lib/lua"

# Check if Lua is already installed
if os.path.exists(DEST):
    print("Lua is already installed.")
    exit(0)

print(f"Downloading Lua {LUA_VERSION}...")
urllib.request.urlretrieve(URL, "lua.tar.gz")

print("Extracting...")
with tarfile.open("lua.tar.gz", "r:gz") as tar:
    tar.extractall()

print("Copying source...")
os.makedirs(DEST, exist_ok=True)
src_dir = f"lua-{LUA_VERSION}/src"

for file in os.listdir(src_dir):
    shutil.copy(os.path.join(src_dir, file), DEST)

print("Cleaning up...")
os.remove("lua.tar.gz")
shutil.rmtree(f"lua-{LUA_VERSION}")

print("Lua installed into lib/lua")
