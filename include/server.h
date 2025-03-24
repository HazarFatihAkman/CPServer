#ifndef SERVER_H
#define SERVER_H

#include "utilities.h"
#include <stdbool.h>

#define SIZE_256 256
#define SIZE_1024 1024

#if _WIN32
    #include <WinSock2.h>
#elif __linux__ || __APPLE__
    //TODO : Linux & MACOS
#else
    #error "Unsupported platform"
#endif

typedef enum SERVER_TYPE {
    TCP,
    UDP
};

typedef struct server_client server_client_t;

typedef struct server_config {
    int max_connected_clients;
    char *name;
} server_config_t;

typedef struct server {
    // port to listen. if 0, grab any available port.
    int port;

    // server socket
    #if _WIN32
        SOCKET fd;
    #elif __linux__ || __APPLE__
        int fd;
    #else
        #error "Unsupported platform"
    #endif

    enum SERVER_TYPE type;

    int pid;

    bool ready;

    #if _WIN32
        HANDLE lock;
    #else
        // TODO : LINUX & MACOS
    #endif

    server_config_t *config;
} server_t;

typedef struct packet {    
    int type; // 0 : Data, 1 : Control
    union {
        bool control;
        const char *data;
    } payload;
} packet_t;

extern server_t **active_servers;
extern int server_count;

void server(char*, enum SERVER_TYPE, int, void *(*)(char*));
void client(int);

packet_t *create_data_pack(const char*);
packet_t *create_control_pack(bool);
char *server_type(enum SERVER_TYPE);
#endif