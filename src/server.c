#include "../include/server.h"

static char *server_type(enum SERVER_TYPE type) {
    switch (type) {
        case 0:
            return "TCP";
        case 1:
            return "UDP";
        default:
            break;
    }
}

int init_network() {
    #if _WIN32
        WSADATA wsaDATA;
        int result;
        int attemps;
        while ((result = WSAStartup(MAKEWORD(2, 2), & wsaDATA)) != 0 && attemps < MAX_ATTEMPS) {
            fprintf(stderr, "WSAStartup failed with error %d. Retrying... (%zu/%zu)\n", result, attemps + 1, MAX_ATTEMPS);
            ++attemps;
        }

        if (result != 0) {
            fprintf(stderr, "WSAStartup failed after %zu attemps. Exiting...\n", attemps);
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

    server_t *server = (server_t*)default_allocator.allocate(sizeof(server_t));

    server->type = type;
    server->port = 0;
    server->max_connected_clients = max_connected_clients;
    server->ready = false;

    #if _WIN32
        server->fd = (SOCKET)default_allocator.allocate(sizeof(SOCKET));
    #elif __linux__ || __APPLE__
        server->fd = 0;
    #else
        #error "Unsupported platform"
    #endif
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