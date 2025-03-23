#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345
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
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];

    for (int i = 0; i < 3; i++) {
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
        serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);  // Use server IP (127.0.0.1 for localhost)
        serverAddr.sin_port = htons(SERVER_PORT);

        // Send a message to the server
        const char *message = "Hello, UDP Server!";
        sendto(udpSocket, message, strlen(message), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

        // Receive the response from the server
        int addrLen = sizeof(serverAddr);
        int bytesReceived = recvfrom(udpSocket, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&serverAddr, &addrLen);
        if (bytesReceived == SOCKET_ERROR) {
            printf("Error receiving response: %ld\n", WSAGetLastError());
            closesocket(udpSocket);
            WSACleanup();
            return 1;
        }
        buffer[bytesReceived] = '\0';
        printf("Received from server: %s\n", buffer);

        // Clean up
        closesocket(udpSocket);
    }
    WSACleanup();
    return 0;
}
