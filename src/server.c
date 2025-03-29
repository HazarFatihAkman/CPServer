#include "../include/server.h"

void handle_recv(server_t *, struct sockaddr_in *, const char*(*)(char*));

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
        while ((result = WSAStartup(MAKEWORD(2, 2), & wsaDATA)) != 0 && attempts < MAX_ATTEMPTS) {
            fprintf(stderr, "WSAStartup failed with error %d. Retrying... (%zu/%zu)\n", result, attempts + 1, MAX_ATTEMPTS);
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

void init_server(char *name, enum SERVER_TYPE type, int max_connected_clients, const char*(*handler)(char*)) {
    if (init_network() != 1) {
        exit(EXIT_FAILURE);
    }
    int attempts = 0;
    struct sockaddr_in server_addr;
    server_t *server = (server_t*)default_allocator.allocate(sizeof(server_t));

    server->config = (server_config_t*)default_allocator.allocate(sizeof(server_config_t));
    server->config->max_connected_clients = max_connected_clients;
    server->config->name = name;

    server->type = type;
    server->port = 0;
    server->ready = false;

    #if _WIN32
        server->fd = (SOCKET)default_allocator.allocate(sizeof(SOCKET));
        int protocol = (server->type == 0) ? IPPROTO_TCP : IPPROTO_UDP;
        while ((server->fd = socket(AF_INET, (server->type == 0) ? SOCK_STREAM : SOCK_DGRAM, protocol)) == INVALID_SOCKET && attempts < MAX_ATTEMPTS) {
            ++attempts;
            int error_code = WSAGetLastError();
            fprintf(stderr, "Socket creation failed with error code %d. Retrying... (%d/%d)\n", error_code, attempts, MAX_ATTEMPTS);
            #if _WIN32
                Sleep(100);
            #elif __linux__ && __APPLE__
                // TODO : Linux && MACOS
            #endif
            if (attempts == MAX_ATTEMPTS - 1) {
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

    while (bind(server->fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0 && attempts < MAX_ATTEMPTS) {
        ++attempts;
        fprintf(stderr, "Bind failed. Retrying... (%zu/%zu)\n", attempts, MAX_ATTEMPTS);
        #if _WIN32
            Sleep(100);
        #elif __linux__ && __APPLE__
            // TODO : Linux && MACOS
        #endif
        if (attempts == MAX_ATTEMPTS - 1) {
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
        while (getsockname(server->fd, (struct sockaddr*)&server_addr, &len) == -1 && attempts < MAX_ATTEMPTS) {
            ++attempts;
            fprintf(stderr, "Getsockname failed. Retrying... (%zu/%zu)\n", attempts, MAX_ATTEMPTS);
            #if _WIN32
                Sleep(100);
            #elif __linux__ && __APPLE__
                // TODO : Linux && MACOS
            #endif
            if (attempts == MAX_ATTEMPTS - 1) {
                break;
            }
        }
    }

    server->port = ntohs(server_addr.sin_port);

    if (server->type == TCP) {
        attempts = 0;

        while ((listen(server->fd, server->config->max_connected_clients) < 0) && attempts < MAX_ATTEMPTS) {
            ++attempts;
            fprintf(stderr, "Listen failed. Retrying... (%zu/%zu)\n", attempts, MAX_ATTEMPTS);
            #if _WIN32
                Sleep(100);
            #elif __linux__ && __APPLE__
                // TODO : Linux && MACOS
            #endif

            if (attempts == MAX_ATTEMPTS - 1) {
               break;
            }
        }
    }

    server->ready = true;
    fprintf(stderr, "Server is ready on port %d\n", server->port);
    push_server(server);
    handle_recv(server, &server_addr, handler);
}

void handle_recv(server_t *server, struct sockaddr_in *server_addr, const char*(*handler)(char*)) {
    char buffer[SIZE_1024];
    struct sockaddr_in client_addr;
    SOCKET client_socket;
    int client_addr_len = sizeof(client_addr);
    int attempts = 0, byte_received;

    if (server->type == TCP) {
        while (1) {
            #if _WIN32
                init_network();
                client_socket = accept(server->fd, (struct sockaddr*)&client_addr, &client_addr_len);
                if (client_socket == INVALID_SOCKET) {
                    fprintf(stderr, "Accept failed with error: %d\n", WSAGetLastError());
                    continue;
                }
                else {
                    byte_received = recv(client_socket, buffer, SIZE_1024, 0);
                    if (byte_received == SOCKET_ERROR) {
                        fprintf(stderr, "ERROR RECEIVING DATA %ld\n", WSAGetLastError());
                        closesocket(client_socket);
                        continue;
                    }
                    else {
                        buffer[byte_received] = '\0';
                        const char *response = handler(buffer);
                        int byte_sent = send(client_socket, response, strlen(response), 0);
                        if (byte_sent == SOCKET_ERROR) {
                            fprintf(stderr, "Error sending response : %d\n", WSAGetLastError());
                        }

                        if (shutdown(client_socket, SD_SEND) == SOCKET_ERROR) {
                                    printf("Shutdown failed. Error Code : %d\n", WSAGetLastError());
                        }
                    }
                }

            #elif __linux__ || __APPLE__
                // TODO : Linux & MACOS
            #endif
        }
    }
    else if (server->type == UDP) {
        while (1) {
            #if _WIN32
                // Receive a message from the client
                byte_received = recvfrom(server->fd, (char *)buffer, SIZE_1024, 0, (struct sockaddr *)&client_addr, &client_addr_len);
                if (byte_received < 0) {
                    perror("Failed to receive message");
                    continue;
                }

                // Null-terminate the received message
                buffer[byte_received] = '\0';
                printf("Received message: %s\n", buffer);

                // Send a response to the client
                const char *response = handler(buffer);
                sendto(server->fd, (const char *)response, strlen(response), 0, (const struct sockaddr *)&client_addr, client_addr_len);
                printf("Response sent to client\n");
            #elif __linux__ || __APPLE__
            
            #endif
        }
    
        // while (1) {
        //     #if _WIN32
        //         byte_received = recvfrom(server->fd, buffer, SIZE_1024, 0, (struct sockaddr*)&client_addr, &add_len);
        //         if (byte_received == SOCKET_ERROR) {
        //             fprintf(stderr, "ERROR RECEIVING DATA %ld\n", WSAGetLastError());
        //             continue;
        //         }

        //         buffer[byte_received] = '\0';
        //         if (byte_received > 0){ //If there are data
        //             //Print information for received data
        //             printf("Server: Total Bytes received: %d\n", byte_received);
        //             printf("Server: The data is: %s\n", buffer);
        //             printf("\n");
        //         }
        //         else if (byte_received <= 0){ //If the buffer is empty
        //             //Print error message
        //             printf("Server: Connection closed with error code: %ld\n", WSAGetLastError());
        //         }
        //         else{ //If error
        //             //Print error message
        //             printf("Server: recvfrom() failed with error code: %d\n", WSAGetLastError());
        //         }

        //         const char *response = handler(buffer);
        //         sendto(client_socket, "response", strlen("response"), 0, (struct sockaddr *)&client_addr, add_len);
        //     #elif __linux__ || __APPLE__
        //         // TODO : Linux & MACOS
        //     #endif
        // }
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