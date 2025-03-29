#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <winsock2.h>
#include <Ws2tcpip.h>  // Include this for InetPton

#pragma comment(lib, "ws2_32.lib")  // Link the Winsock library

#define SERVER_IP "127.0.0.1"  // Server's IP address (use actual IP address)
#define SERVER_PORT 64185       // Server's port
#define MAX_BUFFER_SIZE 1024

int main() {
    WSADATA wsaData;
    SOCKET client_fd;
    struct sockaddr_in server_addr;
    char buffer[MAX_BUFFER_SIZE];
    char *message = "{\"test\":\"hello from client\"}";
    int server_len, recv_len;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed with error %d\n", WSAGetLastError());
        return 1;
    }

    // Create UDP socket
    client_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (client_fd == INVALID_SOCKET) {
        printf("Socket creation failed with error %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // Set up the server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);

    // Check for Windows version compatibility
    #if defined(_WIN32) || defined(_WIN64)
    // Use InetPton for Windows Vista and later
    if (InetPton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        printf("Invalid address or address not supported\n");
        closesocket(client_fd);
        WSACleanup();
        return 1;
    }
    #else
    // Use inet_addr for older versions of Windows (pre-Vista)
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    if (server_addr.sin_addr.s_addr == INADDR_NONE) {
        printf("Invalid address or address not supported\n");
        closesocket(client_fd);
        WSACleanup();
        return 1;
    }
    #endif

    // Send message to the server
    sendto(client_fd, message, strlen(message), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
    printf("Message sent to server: %s\n", message);

    // Receive response from the server
    server_len = sizeof(server_addr);
    recv_len = recvfrom(client_fd, buffer, MAX_BUFFER_SIZE, 0, (struct sockaddr*)&server_addr, &server_len);
    if (recv_len == SOCKET_ERROR) {
        printf("Failed to receive response with error %d\n", WSAGetLastError());
        closesocket(client_fd);
        WSACleanup();
        return 1;
    }
    buffer[recv_len] = '\0';  // Null-terminate the received data
    printf("Response from server: %s\n", buffer);

    // Clean up and close the socket
    closesocket(client_fd);
    WSACleanup();
    return 0;
}
