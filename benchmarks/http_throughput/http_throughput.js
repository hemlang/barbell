#!/usr/bin/env node
// HTTP throughput benchmark - JavaScript (Node.js) implementation

const http = require('http');

const port = parseInt(process.argv[2]);

if (!port) {
    console.error('Usage: node http_throughput.js <port>');
    process.exit(1);
}

const response = JSON.stringify({ message: 'Hello World!' });
const headers = {
    'Content-Type': 'application/json',
    'Content-Length': Buffer.byteLength(response),
    'Connection': 'close'
};

const server = http.createServer((req, res) => {
    if (req.method === 'GET') {
        res.writeHead(200, headers);
        res.end(response);
    } else {
        res.writeHead(405);
        res.end();
    }
});

server.listen(port, () => {
    // Signal ready
    console.log(`READY ${port}`);
});

process.on('SIGTERM', () => {
    server.close(() => process.exit(0));
});

process.on('SIGINT', () => {
    server.close(() => process.exit(0));
});
