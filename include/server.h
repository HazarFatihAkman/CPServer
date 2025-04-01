#ifndef SERVER_H
#define SERVER_H

#include "http_content.h"

#if _WIN32
    #include <WinSock2.h>
    #define SOCKET_TYPE SOCKET
    #define INVALID_SOCK INVALID_SOCKET
    #define SOCK_ERROR SOCKET_ERROR
    #define ADDR_TYPE int
    #define SHUTDOWN_SEND SD_SEND
    #define CLOSE_SOCK closesocket
    #pragma comment(lib, "ws2_32.lib")
#elif __linux__ || __APPLE__
    #include <unistd.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <errno.h>
    #define SOCKET_TYPE int
    #define INVALID_SOCK -1
    #define SOCK_ERROR -1
    #define ADDR_TYPE socklen_t
    #define SHUTDOWN_SEND SHUT_WR
    #define CLOSE_SOCK close
#else
    #error "Unsupported platform"
#endif

typedef enum server_enum_type {
    TCP,
    UDP
} server_type;

typedef struct socket_info {
    // port to listen. if 0, grab any available port.
    int port;
    SOCKET_TYPE fd;
    struct sockaddr_in addr;
    ADDR_TYPE addr_len;
} socket_info_t;

typedef struct server_config {
    int max_connected_clients;
    char *name;
} server_config_t;

typedef struct server {
    int pid;
    server_type type;
    socket_info_t *socket;
    server_config_t *config;
    bool ready;
    // TODO : IDK HOW TO USE LOCK MECHANISM EXACTLY.
    // LOCK IS 

    // #if _WIN32
    //     HANDLE lock;
    // #else
    //     // TODO : LINUX & MACOS
    // #endif

} server_t;

extern server_t **active_servers;
extern int server_count;

void init_server(char*, server_type, int, const char*(*)(char*));
void client(int);

char *server_type_name(server_type);
#endif