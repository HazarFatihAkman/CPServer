#include "../include/server.h"

void handle_recv(server_t *, const char*(*)(char*));

server_t **active_servers = NULL;
int server_count = 0;

char *server_type_name(server_type type) {
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

void init_server(char *name, server_type type, int max_connected_clients, const char*(*handler)(char*)) {
    if (init_network() != 1) {
        exit(EXIT_FAILURE);
    }

    int attempts = 0;
    server_t *server = (server_t*)default_allocator.allocate(sizeof(server_t));

    server->config = (server_config_t*)default_allocator.allocate(sizeof(server_config_t));
    server->socket = (socket_info_t*)default_allocator.allocate(sizeof(socket_info_t));
    server->config->max_connected_clients = max_connected_clients;
    server->config->name = name;

    server->type = type;
    server->socket->port = 0;
    server->ready = false;

    int protocol = (server->type == 0) ? IPPROTO_TCP : IPPROTO_UDP;
    int sock_type = (server->type == 0) ? SOCK_STREAM : SOCK_DGRAM;

    #if _WIN32
        server->socket.fd = (SOCKET)default_allocator.allocate(sizeof(SOCKET));
    #elif __linux__ || __APPLE__
        server->socket->fd = 0;
    #else
        #error "Unsupported platform"
    #endif
    
    while ((server->socket->fd = socket(AF_INET, sock_type, protocol)) == INVALID_SOCK && attempts < MAX_ATTEMPTS) {
        ++attempts;
        #if __WIN32
            fprintf(stderr, "Socket creation failed with error code %d. Retrying... (%d/%d)\n", WSAGetLastError(), attempts, MAX_ATTEMPTS);
        #elif __linux__ || __APPLE__
            fprintf(stderr, "Socket creation failed with error code %s. Retrying... (%d/%d)\n", strerror(errno), attempts, MAX_ATTEMPTS);
        #endif

        if (attempts == MAX_ATTEMPTS - 1) {
            break;
        }
    }

    attempts = 0;
    memset(&server->socket->addr, 0, sizeof(server->socket->addr));
    server->socket->addr.sin_family = AF_INET;
    server->socket->addr.sin_addr.s_addr = INADDR_ANY;  // Bind to any available interface
    server->socket->addr.sin_port = htons(0);  // Let the OS choose the port

    while (bind(server->socket->fd, (struct sockaddr *)&server->socket->addr, sizeof(server->socket->addr)) < 0 && attempts < MAX_ATTEMPTS) {
        ++attempts;
        fprintf(stderr, "Bind failed. Retrying... (%d/%d)\n", attempts, MAX_ATTEMPTS);

        if (attempts == MAX_ATTEMPTS - 1) {
            break;
        }
    }

    server->socket->addr_len = sizeof(server->socket->addr);

    attempts = 0;
    if (server->socket->port == 0) {
        while (getsockname(server->socket->fd, (struct sockaddr*)&server->socket->addr, &server->socket->addr_len) == -1 && attempts < MAX_ATTEMPTS) {
            ++attempts;
            fprintf(stderr, "Getsockname failed. Retrying... (%d/%d)\n", attempts, MAX_ATTEMPTS);
  
            if (attempts == MAX_ATTEMPTS - 1) {
                break;
            }
        }
    }

    server->socket->port = ntohs(server->socket->addr.sin_port);

    if (server->type == TCP) {
        attempts = 0;

        while (listen(server->socket->fd, server->config->max_connected_clients) < 0 && attempts < MAX_ATTEMPTS) {
            ++attempts;
            fprintf(stderr, "Listen failed. Retrying... (%d/%d)\n", attempts, MAX_ATTEMPTS);

            if (attempts == MAX_ATTEMPTS - 1) {
               break;
            }
        }
    }

    server->ready = true;
    fprintf(stderr, "Server is ready on port %d\n", server->socket->port);
    push_server(server);
    handle_recv(server, handler);
}

void handle_recv(server_t *server, const char*(*handler)(char*)) {
    char buffer[SIZE_1024];
    socket_info_t *client_socket = (socket_info_t*)default_allocator.allocate(sizeof(socket_info_t));
    client_socket->addr_len = sizeof(client_socket->addr);
    int attempts = 0, byte_received;
    if (server->type == TCP) {
        while (1) {
            #if _WIN32
                init_network();
            #endif

            client_socket->fd = accept(server->socket->fd, (struct sockaddr*)&client_socket->addr, &client_socket->addr_len);
            if (client_socket->fd == INVALID_SOCK) {
                #if __WIN32
                    fprintf(stderr, "Accept failed with error: %d\n", WSAGetLastError());
                #elif __linux__ || __APPLE__
                    fprintf(stderr, "Accept failed with error: %s\n", strerror(errno));
                #endif
                continue;
            }
            else {
                byte_received = recv(client_socket->fd, buffer, SIZE_1024, 0);
                if (byte_received == SOCK_ERROR) {
                    #if __WIN32
                        fprintf(stderr, "Error receiving data %d\n", WSAGetLastError());
                    #elif __linux__ || __APPLE__
                        fprintf(stderr, "Error receiving data %s\n", strerror(errno));
                    #endif
                    CLOSE_SOCK(client_socket->fd);
                    continue;
                }
                else {
                    buffer[byte_received] = '\0';
                    const char *response = handler(buffer);
                    int byte_sent = send(client_socket->fd, response, strlen(response), 0);

                    CLOSE_SOCK(client_socket->fd);
                    if (byte_sent == SOCK_ERROR) {
                        #if __WIN32
                            fprintf(stderr, "Error sending response %d\n", WSAGetLastError());
                        #elif __linux__ || __APPLE__
                            fprintf(stderr, "Error sending response %s\n", strerror(errno));
                        #endif
                    }
                }
            }
        }
    }
    else if (server->type == UDP) {
        while (1) {
            byte_received = recvfrom(server->socket->fd, (char *)buffer, SIZE_1024, 0, (struct sockaddr *)&client_socket->addr, &client_socket->addr_len);
            if (byte_received < 0) {
                perror("Failed to receive message");
                continue;
            }

            // Null-terminate the received message
            buffer[byte_received] = '\0';
            printf("Received message: %s\n", buffer);

                // Send a response to the client
            const char *response = handler(buffer);
            sendto(server->socket->fd, (const char *)response, strlen(response), 0, (const struct sockaddr *)&client_socket->addr, client_socket->addr_len);
            printf("Response sent to client\n");
        }
    }

    free(client_socket);
}

void client(int port) {
    // TODO
}
