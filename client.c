#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 512

int main(void) {
    int socket_fd;
    struct sockaddr_in server_address;
    char filename[BUFFER_SIZE];
    char response[BUFFER_SIZE];

    // Create socket
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_fd < 0) {
        perror("Socket error");
        return 1;
    }

    // Set server address
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr);

    // Connect to server
    if (connect(socket_fd, (struct sockaddr *)&server_address,
                sizeof(server_address)) < 0) {
        perror("Connection error");
        close(socket_fd);
        return 1;
    }

    // Get filename
    printf("Enter filename: ");

    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        close(socket_fd);
        return 1;
    }

    // Remove newline
    filename[strcspn(filename, "\r\n")] = '\0';

    // Reject empty filename
    if (strlen(filename) == 0) {
        printf("Error: Empty filename.\n");
        close(socket_fd);
        return 1;
    }

    // Send filename
    if (send(socket_fd, filename, strlen(filename), 0) < 0) {
        perror("Send error");
        close(socket_fd);
        return 1;
    }

    // Receive response
    int bytes_received;

    while ((bytes_received = recv(socket_fd, response,
                                  sizeof(response) - 1, 0)) > 0) {

        response[bytes_received] = '\0';
        printf("%s", response);
    }

    // Close socket
    close(socket_fd);

    return 0;
}