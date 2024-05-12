#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

int add(int a, int b) {
    return a + b;
}

int main() {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    int buffer[2]; // Changed buffer size to 2 since we're receiving two integers

    // Create socket
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("[-]Socket error");
        exit(1);
    }
    printf("[+]Server socket created.\n");

    // Configure settings
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(7799);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket
    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("[-]Bind error");
        exit(1);
    }
    printf("[+]Bind to the port number: 7799\n");

    // Listen for connections
    listen(server_sock, 5);
    printf("[+]Listening...\n");

    addr_size = sizeof(client_addr);

    // Accept connection from client
    client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &addr_size);
    if (client_sock < 0) {
        perror("[-]Accept error");
        exit(1);
    }
    printf("[+]Client connected.\n");

    // Receive data
    recv(client_sock, buffer, sizeof(buffer), 0);

    int a = buffer[0];
    int b = buffer[1];

    // Perform operation (assuming function ID is for addition)
    int result = add(a, b);

    // Send result back to client
    send(client_sock, &result, sizeof(result), 0);
    printf("[+]Data sent: %d\n", result);

    // Close the sockets
    close(client_sock);
    close(server_sock);
    printf("[+]Connection closed.\n");

    return 0;
}
