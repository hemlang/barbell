#!/usr/bin/env python3
"""
HTTP throughput benchmark client.
Makes N requests to a server and reports requests/second.
Uses one connection per request for consistent benchmarking.
"""

import sys
import time
import socket
import argparse

def make_requests(host, port, num_requests):
    """Make HTTP requests and return elapsed time in seconds."""
    request = (
        f"GET / HTTP/1.1\r\n"
        f"Host: {host}:{port}\r\n"
        f"Connection: close\r\n"
        "\r\n"
    ).encode()

    start = time.perf_counter()
    completed = 0

    for _ in range(num_requests):
        try:
            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            sock.settimeout(5.0)
            sock.connect((host, port))
            sock.sendall(request)

            # Read full response until connection closes
            response = b""
            while True:
                chunk = sock.recv(4096)
                if not chunk:
                    break
                response += chunk

            sock.close()
            completed += 1
        except (socket.error, socket.timeout) as e:
            # Skip failed requests
            pass

    elapsed = time.perf_counter() - start
    return completed, elapsed

def main():
    parser = argparse.ArgumentParser(description='HTTP throughput benchmark client')
    parser.add_argument('--host', default='127.0.0.1', help='Server host')
    parser.add_argument('--port', type=int, required=True, help='Server port')
    parser.add_argument('--requests', type=int, default=10000, help='Number of requests')

    args = parser.parse_args()

    completed, elapsed = make_requests(args.host, args.port, args.requests)

    if completed > 0:
        rps = completed / elapsed
        print(f"{rps:.2f}")
    else:
        print("0.00")

if __name__ == '__main__':
    main()
