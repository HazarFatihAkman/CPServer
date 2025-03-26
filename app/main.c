#include "../include/server.h"

const char* handle_get(char* received_data) {
    http_content_t *content = get_http_content(received_data);
    // printf("%d\n", content->content_len);
    // printf("%s\n", content_type_name(content->connection_type));
    // printf("%s\n", http_request_name(content->request_type));
    // printf("%s\n", content->content);
    return create_response(OK, APPLICATION_JSON, "{\"status\": \"success\", \"message\": \"Message received\"}");
}

int main(void) {
    server("Server - 1", TCP, 5, handle_get);
    // server("Server - 2", UDP, 10, NULL);
    // server("Server - 3", TCP, 12, NULL);

    while (1) {
        for_loop(i, server_count) {
            printf("Active Server Port : %d | Type : %s\n", active_servers[i]->port, server_type(active_servers[i]->type));
            printf("%s - %d\n", active_servers[i]->config->name, active_servers[i]->config->max_connected_clients);
        }

        packet_t *packet = create_data_pack("DATA PACKAGE");
        printf("%s\n", packet->payload.data);
        Sleep(5000);
    }
    return 1;
}
