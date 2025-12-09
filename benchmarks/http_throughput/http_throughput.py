#!/usr/bin/env python3
# HTTP throughput benchmark - Python implementation

import sys
import signal
from http.server import HTTPServer, BaseHTTPRequestHandler
import json

class BenchmarkHandler(BaseHTTPRequestHandler):
    # Disable logging for performance
    def log_message(self, format, *args):
        pass

    def do_GET(self):
        self.send_response(200)
        self.send_header('Content-Type', 'application/json')
        self.send_header('Connection', 'close')
        response = b'{"message":"Hello World!"}'
        self.send_header('Content-Length', len(response))
        self.end_headers()
        self.wfile.write(response)

def main():
    if len(sys.argv) < 2:
        print(f"Usage: {sys.argv[0]} <port>", file=sys.stderr)
        sys.exit(1)

    port = int(sys.argv[1])
    server = HTTPServer(('', port), BenchmarkHandler)

    def shutdown(signum, frame):
        server.shutdown()

    signal.signal(signal.SIGTERM, shutdown)
    signal.signal(signal.SIGINT, shutdown)

    # Signal ready
    print(f"READY {port}", flush=True)

    server.serve_forever()

if __name__ == '__main__':
    main()
