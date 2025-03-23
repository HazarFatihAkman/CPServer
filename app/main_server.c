#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

#define PORT 12345
#define BUFFER_SIZE 1024

void initializeWinsock() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed\n");
        exit(1);
    }
}

int main() {
    initializeWinsock();

    SOCKET udpSocket;
    struct sockaddr_in serverAddr, clientAddr;
    int addrLen = sizeof(clientAddr);
    char buffer[BUFFER_SIZE];

    // Create a UDP socket
    udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (udpSocket == INVALID_SOCKET) {
        printf("Error creating socket: %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // Set up the server address structure
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;  // Listen on all interfaces
    serverAddr.sin_port = htons(PORT);        // Listen on port 12345

    // Bind the socket to the address and port
    if (bind(udpSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        printf("Bind failed: %ld\n", WSAGetLastError());
        closesocket(udpSocket);
        WSACleanup();
        return 1;
    }

    printf("Server listening on port %d...\n", PORT);

    // Loop to continuously receive and process data from clients
    while (1) {
        int bytesReceived = recvfrom(udpSocket, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&clientAddr, &addrLen);
        if (bytesReceived == SOCKET_ERROR) {
            printf("Error receiving data: %ld\n", WSAGetLastError());
            continue;
        }

        // Null-terminate the received data to treat it as a string
        buffer[bytesReceived] = '\0';

        // Print received data
        printf("Received message from %s:%d: %s\n",
               inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port), buffer);

        // Optionally, you can send a response to the client
        const char *response = "Message received";
        sendto(udpSocket, response, strlen(response), 0, (struct sockaddr*)&clientAddr, addrLen);
    }

    // Clean up (this code is never reached, since the server runs forever)
    closesocket(udpSocket);
    WSACleanup();
    return 0;
}
