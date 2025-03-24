#include "../include/server.h"

int main(void) {
    server("Server - 1", TCP, 5, NULL);
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
