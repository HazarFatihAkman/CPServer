#include "../include/server.h"

int main(void) {
    server(TCP, 5);
    server(UDP, 10);
    server(TCP, 12);
    while (1) {
        for_loop(i, server_count) {
            printf("Active Server Port : %d | Type : %s\n", active_servers[i]->port, server_type(active_servers[i]->type));
        }
        packet_t *packet = create_data_pack("DATA PACKAGE");
        printf("%s\n", packet->payload.data);
        Sleep(5000);
    }
    return 1;
}
