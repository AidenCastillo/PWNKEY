#include "engine/LuaWifi.h"
#ifdef ESP32
#include <Arduino.h>
#include <WiFi.h>

static int connect(lua_State* L) {
    const char* ssid = luaL_checkstring(L, 1);
    const char* pass = luaL_checkstring(L, 2);
    WiFi.begin(ssid, pass);
    return 0;
}

static int status(lua_State* L) {
    lua_pushinteger(L, WiFi.status());
    return 1;
}

static int scan(lua_State* L) {
    int n = WiFi.scanNetworks();
    lua_newtable(L);
    for (int i = 0; i < n; i++) {
        lua_pushinteger(L, i + 1);
        lua_pushstring(L, WiFi.SSID(i).c_str());
        lua_settable(L, -3);
    }
    return 1;
}

void LuaWifi::registerModule(lua_State* L) {
    lua_newtable(L);

    lua_pushcfunction(L, connect);
    lua_setfield(L, -2, "connect");

    lua_pushcfunction(L, status);
    lua_setfield(L, -2, "status");

    lua_pushcfunction(L, scan);
    lua_setfield(L, -2, "scan");

    lua_setglobal(L, "wifi");
}

#endif

#ifdef NATIVE

#include <iostream>

#if defined(_WIN32)
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
#endif
// Although wifi, native will utilize sockets, and other networking libraries

static int createSocketServer(lua_State* L) {
    int port = luaL_checkinteger(L, 1);
    std::cout << "[NATIVE] Creating socket server on port: " << port << std::endl;

#if defined(_WIN32)
    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        lua_pushnil(L);
        lua_pushstring(L, "WSAStartup failed");
        return 2;
    }
#endif

    // Create socket
    int server_fd =
#if defined(_WIN32)
        socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
#else
        socket(AF_INET, SOCK_STREAM, 0);
#endif

    if (server_fd < 0) {
        lua_pushnil(L);
        lua_pushstring(L, "socket creation failed");
        return 2;
    }

    // Allow port reuse
    int opt = 1;
#if defined(_WIN32)
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));
#else
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
#endif

    // Bind
    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
#if defined(_WIN32)
        closesocket(server_fd);
        WSACleanup();
#else
        close(server_fd);
#endif
        lua_pushnil(L);
        lua_pushstring(L, "bind failed");
        return 2;
    }

    // Listen
    if (listen(server_fd, 3) < 0) {
#if defined(_WIN32)
        closesocket(server_fd);
        WSACleanup();
#else
        close(server_fd);
#endif
        lua_pushnil(L);
        lua_pushstring(L, "listen failed");
        return 2;
    }

    std::cout << "[NATIVE] Server listening on port " << port << "..." << std::endl;

    // Accept
    socklen_t addr_len = sizeof(address);
    int client_socket = accept(server_fd, (struct sockaddr*)&address, &addr_len);
    if (client_socket < 0) {
#if defined(_WIN32)
        closesocket(server_fd);
        WSACleanup();
#else
        close(server_fd);
#endif
        lua_pushnil(L);
        lua_pushstring(L, "accept failed");
        return 2;
    }

    // Read
    char buffer[1024] = {0};
#if defined(_WIN32)
    int bytes = recv(client_socket, buffer, sizeof(buffer), 0);
#else
    int bytes = read(client_socket, buffer, sizeof(buffer));
#endif

    std::string received = (bytes > 0) ? std::string(buffer, bytes) : "";

    std::cout << "[NATIVE] Client says: " << received << std::endl;

    // Cleanup
#if defined(_WIN32)
    closesocket(client_socket);
    closesocket(server_fd);
    WSACleanup();
#else
    close(client_socket);
    close(server_fd);
#endif

    lua_pushstring(L, received.c_str());
    return 1;
}

void LuaWifi::registerModule(lua_State* L) {
    lua_newtable(L);

    lua_pushcfunction(L, [](lua_State* L) -> int {
        const char* ssid = luaL_checkstring(L, 1);
        const char* pass = luaL_checkstring(L, 2);
        std::cout << "[NATIVE] WiFi connect: " << ssid << " with password: " << pass << std::endl;
        return 0;
    });
    lua_setfield(L, -2, "connect");

    lua_pushcfunction(L, [](lua_State* L) -> int {
        std::cout << "[NATIVE] WiFi status requested" << std::endl;
        lua_pushinteger(L, 1); // Simulate connected status
        return 1;
    });
    lua_setfield(L, -2, "status");

    lua_pushcfunction(L, [](lua_State* L) -> int {
        std::cout << "[NATIVE] WiFi scan requested" << std::endl;
        lua_newtable(L);
        for (int i = 0; i < 3; i++) {
            lua_pushinteger(L, i + 1);
            lua_pushstring(L, ("Network_" + std::to_string(i + 1)).c_str());
            lua_settable(L, -3);
        }
        return 1;
    });
    lua_setfield(L, -2, "scan");

    lua_pushcfunction(L, createSocketServer);
    lua_setfield(L, -2, "createSocketServer");

    lua_setglobal(L, "wifi");
}


#endif
