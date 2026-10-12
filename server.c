#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define BUFFER_SIZE 512
#define MAX_FILE_SIZE 255

int main(void) {
    int server_socket, client_socket;
    struct sockaddr_in server_address;
    char filename[BUFFER_SIZE];
    char file_data[MAX_FILE_SIZE + 1];

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket < 0) {
        perror("Socket error");
        return 1;
    }

    // Set server address
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    // Bind socket
    if (bind(server_socket, (struct sockaddr *)&server_address,
             sizeof(server_address)) < 0) {
        perror("Bind error");
        close(server_socket);
        return 1;
    }

    // Listen for clients
    if (listen(server_socket, 5) < 0) {
        perror("Listen error");
        close(server_socket);
        return 1;
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        // Accept client
        client_socket = accept(server_socket, NULL, NULL);

        if (client_socket < 0) {
            perror("Accept error");
            continue;
        }

        // Receive filename
        int bytes_received = recv(client_socket, filename,
                                  sizeof(filename) - 1, 0);

        if (bytes_received <= 0) {
            close(client_socket);
            continue;
        }

        // Always terminate the string
        filename[bytes_received] = '\0';

        // Reject payloads that fill the entire buffer
        if (bytes_received >= sizeof(filename) - 1) {
            send(client_socket, "Error: Filename is too long.\n", 29, 0);
            close(client_socket);
            continue;
        }

        // Remove newline
        filename[strcspn(filename, "\r\n")] = '\0';

        // Reject empty filename
        if (strlen(filename) == 0) {
            send(client_socket, "Error: Empty filename.\n", 23, 0);
            close(client_socket);
            continue;
        }

        // Reject null bytes
        if (memchr(filename, '\0', bytes_received) != NULL &&
            strlen(filename) != (size_t)bytes_received) {
            send(client_socket, "Error: Invalid filename.\n", 25, 0);
            close(client_socket);
            continue;
        }

        // Reject path traversal and absolute paths
        if (strstr(filename, "..") != NULL ||
            strchr(filename, '/') != NULL ||
            strchr(filename, '\\') != NULL) {
            send(client_socket, "Error: Invalid filename.\n", 25, 0);
            close(client_socket);
            continue;
        }

        // Open requested file
        FILE *file = fopen(filename, "rb");

        if (file == NULL) {
            send(client_socket, "Error: File Not Found.\n", 23, 0);
            close(client_socket);
            continue;
        }

        // Check file size
        fseek(file, 0, SEEK_END);
        long size = ftell(file);
        rewind(file);

        if (size < 0 || size > MAX_FILE_SIZE) {
            send(client_socket, "Error: File is larger than 255 bytes.\n",
                 37, 0);
            fclose(file);
            close(client_socket);
            continue;
        }

        // Read file
        size_t bytes_read = fread(file_data, 1, MAX_FILE_SIZE, file);
        file_data[bytes_read] = '\0';

        // Send file contents
        send(client_socket, file_data, bytes_read, 0);

        fclose(file);
        close(client_socket);
    }

    close(server_socket);
    return 0;
}