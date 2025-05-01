COMP2560 - Lab 10
Makhsuma Khamzaliyeva 110120302

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>

#define BUFFER_SIZE 1024

void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];
    char username[50], password[50];
    int x, y;

    // Receive data from the client
    recv(client_socket, buffer, sizeof(buffer), 0);
    sscanf(buffer, "%s %s %d %d", username, password, &x, &y);

    // Authentication and response
    if (strcmp(username, "comp2560") == 0 && strcmp(password, "f2022") == 0) {
        int result = x + y;
        char response[BUFFER_SIZE];
        sprintf(response, "Result: %d", result);
        send(client_socket, response, strlen(response), 0);
    } else {
        char response[] = "authentication failed!";
        send(client_socket, response, strlen(response), 0);
    }

    close(client_socket); // Close client socket
    exit(0); // Exit child process
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <IP> <PORT>\n", argv[0]);
        return -1;
    }

    char *server_ip = argv[1];
    int server_port = atoi(argv[2]);

    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    // Create server socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        return -1;
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(server_ip);
    server_addr.sin_port = htons(server_port);

    // Bind socket
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Binding failed");
        return -1;
    }

    // Listen for connections
    listen(server_socket, 5);
    printf("Server is running at %s:%d\n", server_ip, server_port);

    // Handle incoming clients
    while (1) {
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
        if (client_socket < 0) {
            perror("Failed to accept connection");
            continue;
        }

        printf("Accepted connection from %s:%d\n",
               inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // Create a child process to handle the client
        if (fork() == 0) {
            close(server_socket); // Child doesn't need the server socket
            handle_client(client_socket);
        }

        close(client_socket); // Parent doesn't need the client socket
    }

    close(server_socket); // Close server socket
    return 0;
}

