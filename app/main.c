#include "../include/server.h"

const char* handle_get(http_content_t *content) {
    return create_response(OK, APPLICATION_JSON, content->content);
}

const char* handle_post(http_content_t *content) {
    return create_response(CREATED, APPLICATION_JSON, content->content);
}

const char* handler(char *received_data) {
    char *keys[1] = {"Postman-Token: "};
    http_content_t *content = http_content(received_data, keys, 1);
    if (content->request_type == GET) {
        return handle_get(content);
    }
    else if (content->request_type == POST) {
        return handle_post(content);
    }
    return NULL;
}

int main(void) {
    init_server("Server - 1", TCP, 5, handler);
    //game_session -> approved -> new thread udp server(auto port) 
    init_server("Server - 2", UDP, 10, NULL);
    // server("Server - 3", TCP, 12, NULL);

    // while (1) {
    //     for_loop(i, server_count) {
    //         printf("Active Server Port : %d | Type : %s\n", active_servers[i]->port, server_type(active_servers[i]->type));
    //         printf("%s - %d\n", active_servers[i]->config->name, active_servers[i]->config->max_connected_clients);
    //     }

    //     packet_t *packet = create_data_pack("DATA PACKAGE");
    //     printf("%s\n", packet->payload.data);
    //     Sleep(5000);
    // }
    return 1;
}
