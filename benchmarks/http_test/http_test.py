import sys

def main():
    n = int(sys.argv[1]) if len(sys.argv) > 1 else 100
    total_bytes = 0

    for i in range(n):
        # Build HTTP request
        request = (
            f"GET /api/data/{i} HTTP/1.1\r\n"
            f"Host: localhost:8080\r\n"
            f"Accept: application/json\r\n"
            f"Connection: keep-alive\r\n"
            f"\r\n"
        )

        # Simulate response
        response = (
            f"HTTP/1.1 200 OK\r\n"
            f"Content-Type: application/json\r\n"
            f"Content-Length: 32\r\n"
            f"\r\n"
            f'{{"id":{i},"status":"ok"}}'
        )

        # Parse response - find body
        parts = response.split("\r\n\r\n", 1)
        if len(parts) == 2:
            total_bytes += len(parts[1])
        total_bytes += len(request) + len(response)

    print(total_bytes)

main()
