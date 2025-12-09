// HTTP throughput benchmark - C implementation
// Simple HTTP server using raw sockets

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>

#define BUFFER_SIZE 4096

static const char *RESPONSE =
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: application/json\r\n"
    "Content-Length: 27\r\n"
    "Connection: close\r\n"
    "\r\n"
    "{\"message\":\"Hello World!\"}";

static int server_fd = -1;
static volatile int running = 1;

void handle_signal(int sig) {
    (void)sig;
    running = 0;
    if (server_fd != -1) {
        close(server_fd);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        return 1;
    }

    int port = atoi(argv[1]);
    struct sockaddr_in address;
    int opt = 1;
    char buffer[BUFFER_SIZE];

    signal(SIGTERM, handle_signal);
    signal(SIGINT, handle_signal);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        return 1;
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        return 1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        return 1;
    }

    if (listen(server_fd, 128) < 0) {
        perror("listen");
        return 1;
    }

    // Signal ready by printing port
    printf("READY %d\n", port);
    fflush(stdout);

    size_t response_len = strlen(RESPONSE);

    while (running) {
        socklen_t addrlen = sizeof(address);
        int client_fd = accept(server_fd, (struct sockaddr *)&address, &addrlen);
        if (client_fd < 0) {
            if (!running) break;
            continue;
        }

        // Read request
        ssize_t bytes_read = read(client_fd, buffer, BUFFER_SIZE - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            // Simple check for HTTP request
            if (strncmp(buffer, "GET ", 4) == 0) {
                write(client_fd, RESPONSE, response_len);
            }
        }

        close(client_fd);
    }

    close(server_fd);
    return 0;
}
