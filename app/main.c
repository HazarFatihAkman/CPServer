#include "../include/server.h"

int main(void) {
    server(TCP);
    packet_t *packet = create_data_pack("DATA PACKAGE");
    printf("%s\n", packet->payload.data);
    return 1;
}