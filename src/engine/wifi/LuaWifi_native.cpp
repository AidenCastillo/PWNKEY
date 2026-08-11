#include <iostream>
#include <cstdio>
#include <lua.hpp>
#include "engine/LuaWifi.h"

#if defined(_WIN32)
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
#endif

extern "C" {
int wifi_connect(lua_State* L) {
    const char* ssid = luaL_checkstring(L, 1);
    const char* pass = luaL_checkstring(L, 2);

    std::cout << "[NATIVE] WiFi connect: " << ssid
              << " with password: " << pass << std::endl;

    return 0;
}

int wifi_status(lua_State* L) {
    std::cout << "[NATIVE] WiFi status requested" << std::endl;

    lua_pushinteger(L, 1); // Simulated "connected"
    return 1;
}

int wifi_scan(lua_State* L) {
    std::cout << "[NATIVE] WiFi scan requested" << std::endl;

    lua_newtable(L);
    for (int i = 0; i < 3; i++) {
        char buffer[32];
        std::snprintf(buffer, sizeof(buffer), "Network_%d", i + 1);

        lua_pushinteger(L, i + 1);
        lua_pushstring(L, buffer);
        lua_settable(L, -3);
    }

    return 1;
}

int wifi_createSocketServer(lua_State* L) {
    int port = luaL_checkinteger(L, 1);
    std::cout << "[NATIVE] Creating socket server on port: " << port << std::endl;

#if defined(_WIN32)
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        lua_pushnil(L);
        lua_pushstring(L, "WSAStartup failed");
        return 2;
    }
#endif

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

    int opt = 1;
#if defined(_WIN32)
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));
#else
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
#endif

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

    char buffer[1024] = {0};
#if defined(_WIN32)
    int bytes = recv(client_socket, buffer, sizeof(buffer), 0);
#else
    int bytes = read(client_socket, buffer, sizeof(buffer));
#endif

    std::string received = (bytes > 0) ? std::string(buffer, bytes) : "";

    std::cout << "[NATIVE] Client says: " << received << std::endl;

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

}
