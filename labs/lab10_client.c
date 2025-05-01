COMP2560 - Lab 10
Makhsuma Khamzaliyeva 110120302

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <SERVER_IP> <SERVER_PORT>\n", argv[0]);
        return -1;
    }

    char *server_ip = argv[1];
    int server_port = atoi(argv[2]);

    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    char username[50], password[50];
    int x, y;

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Socket creation failed");
        return -1;
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(server_ip);
    server_addr.sin_port = htons(server_port);

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection to server failed");
        return -1;
    }

    printf("Connected to the server at %s:%d\n", server_ip, server_port);

    // Communication loop
    while (1) {
        printf("Enter two numbers (x and y, or -1 -1 to quit): ");
        scanf("%d %d", &x, &y);

        if (x == -1 && y == -1) {
            printf("Exiting...\n");
            break;
        }

        printf("Enter username: ");
        scanf("%s", username);
        printf("Enter password: ");
        scanf("%s", password);

        // Send data to the server
        sprintf(buffer, "%s %s %d %d", username, password, x, y);
        send(sock, buffer, strlen(buffer), 0);

        // Receive response from the server
        memset(buffer, 0, sizeof(buffer));
        recv(sock, buffer, sizeof(buffer), 0);
        printf("Server response: %s\n", buffer);
    }

    close(sock); // Close the socket
    return 0;
}

