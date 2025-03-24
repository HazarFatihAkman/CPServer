#include "../include/server.h"

server_t **active_servers = NULL;
int server_count = 0;

char *server_type(enum SERVER_TYPE type) {
    switch (type) {
        case 0:
            return "TCP";
        case 1:
            return "UDP";
        default:
            break;
    }
}

static void push_server(server_t *server) {
    if (active_servers == NULL && server_count == 0) {
        active_servers = (server_t**)default_allocator.allocate(sizeof(server_t**));
        if (active_servers == NULL) {
            fprintf(stderr, "Memory allocation failed");
            exit(EXIT_FAILURE);
        }
        active_servers[server_count] = server;
        ++server_count;
        fprintf(stderr, "Pushed server into active lists\n");
    }
    else {
        active_servers = (server_t**)realloc(active_servers, (server_count + 1) * sizeof(server_t));
        if (active_servers == NULL) {
            fprintf(stderr, "Memory allocation failed");
            exit(EXIT_FAILURE);
        }
        active_servers[server_count] = server;
        ++server_count;
        fprintf(stderr, "Pushed server into active lists\n");
    }
}

int init_network() {
    #if _WIN32
        WSADATA wsaDATA;
        int result;
        int attempts;
        while ((result = WSAStartup(MAKEWORD(2, 2), & wsaDATA)) != 0 && attempts < MAX_ATTEMPS) {
            fprintf(stderr, "WSAStartup failed with error %d. Retrying... (%zu/%zu)\n", result, attempts + 1, MAX_ATTEMPS);
            ++attempts;
        }

        if (result != 0) {
            fprintf(stderr, "WSAStartup failed after %zu attempts. Exiting...\n", attempts);
            return 0;
        }
        return 1;

    #elif __linux__ || __APPLE__
        fprintf(stderr, "Networking initialized successfully\n");
        return 1;
    #else
        #error "Unsupported platform"
    #endif
}

void server(enum SERVER_TYPE type, int max_connected_clients) {
    if (init_network() != 1) {
        exit(EXIT_FAILURE);
    }
    int attempts = 0;
    struct sockaddr_in server_addr;
    server_t *server = (server_t*)default_allocator.allocate(sizeof(server_t));

    server->type = type;
    server->port = 0;
    server->max_connected_clients = max_connected_clients;
    server->ready = false;

    #if _WIN32
        server->fd = (SOCKET)default_allocator.allocate(sizeof(SOCKET));
        int protocol = (server->type == 0) ? IPPROTO_TCP : IPPROTO_UDP;
        while ((server->fd = socket(AF_INET, (server->type == 0) ? SOCK_STREAM : SOCK_DGRAM, protocol)) == INVALID_SOCKET && attempts < MAX_ATTEMPS) {
            ++attempts;
            int error_code = WSAGetLastError();
            fprintf(stderr, "Socket creation failed with error code %d. Retrying... (%d/%d)\n", error_code, attempts, MAX_ATTEMPS);
            Sleep(100);
            if (attempts == MAX_ATTEMPS - 1) {
                break;
            }
        }
    #elif __linux__ || __APPLE__
        server->fd = 0;
    #else
        #error "Unsupported platform"
    #endif

    attempts = 0;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;  // Bind to any available interface
    server_addr.sin_port = htons(0);  // Let the OS choose the port

    while (bind(server->fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0 && attempts < MAX_ATTEMPS) {
        ++attempts;
        fprintf(stderr, "Bind failed. Retrying... (%zu/%zu)\n", attempts, MAX_ATTEMPS);
        Sleep(100);
        if (attempts == MAX_ATTEMPS - 1) {
            break;
        }
    }

    #if _WIN32
        int len = sizeof(server_addr);
    #elif __linux__ || __APPLE__
        server->fd = 0;
        socklen_t len = sizeof(server_addr);
    #else
        #error "Unsupported platform"
    #endif

    attempts = 0;
    if (server->port == 0) {
        while (getsockname(server->fd, (struct sockaddr*)&server_addr, &len) == -1 && attempts < MAX_ATTEMPS) {
            ++attempts;
            fprintf(stderr, "Getsockname failed. Retrying... (%zu/%zu)\n", attempts, MAX_ATTEMPS);
            Sleep(100);
            if (attempts == MAX_ATTEMPS - 1) {
                break;
            }
        }
    }

    server->port = ntohs(server_addr.sin_port);

    if (server->type == TCP) {
         attempts = 0;

         while ((listen(server->fd, server->max_connected_clients) < 0) && attempts < MAX_ATTEMPS) {
            ++attempts;
            fprintf(stderr, "Listen failed. Retrying... (%zu/%zu)\n", attempts, MAX_ATTEMPS);
            Sleep(100);
            if (attempts == MAX_ATTEMPS - 1) {
               break;
            }
         }
    }

    server->ready = true;
    printf("Server is ready on port %d\n", server->port);
    push_server(server);

    while (1) {
        
    }
}

void client(int port) {
    // TODO
}

packet_t *create_data_pack(const char* data) {
    packet_t *packet = NULL;
    while (packet == NULL) {
        packet = (packet_t*)default_allocator.allocate(sizeof(packet_t));

        if (packet == NULL) {
            perror("Failed to allocate memory for packet data\n");
        }
    }
    packet->type = 0;
    packet->payload.data = data;
    return packet;
}

packet_t *create_control_pack(bool control) {
    packet_t *packet = NULL;
    while (packet == NULL) {
        packet = (packet_t*)default_allocator.allocate(sizeof(packet_t));

        if (packet == NULL) {
            perror("Failed to allocate memory for packet control\n");
        }
    }

    packet->type = 1;
    packet->payload.control = control;

    return packet;
}