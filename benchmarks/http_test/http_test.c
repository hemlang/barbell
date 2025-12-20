#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Build an HTTP request and parse a response N times
int main(int argc, char *argv[]) {
    int n = argc > 1 ? atoi(argv[1]) : 100;
    int total_bytes = 0;

    for (int i = 0; i < n; i++) {
        // Build HTTP request
        char request[512];
        int req_len = snprintf(request, sizeof(request),
            "GET /api/data/%d HTTP/1.1\r\n"
            "Host: localhost:8080\r\n"
            "Accept: application/json\r\n"
            "Connection: keep-alive\r\n"
            "\r\n", i);

        // Simulate response
        char response[1024];
        int resp_len = snprintf(response, sizeof(response),
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: application/json\r\n"
            "Content-Length: 32\r\n"
            "\r\n"
            "{\"id\":%d,\"status\":\"ok\"}", i);

        // Parse response - find headers end and extract status
        char *body = strstr(response, "\r\n\r\n");
        if (body) {
            body += 4;
            total_bytes += strlen(body);
        }
        total_bytes += req_len + resp_len;
    }

    printf("%d\n", total_bytes);
    return 0;
}
