function main() {
    const n = parseInt(process.argv[2]) || 100;

    const start = performance.now();
    let totalBytes = 0;

    for (let i = 0; i < n; i++) {
        // Build HTTP request
        const request =
            `GET /api/data/${i} HTTP/1.1\r\n` +
            `Host: localhost:8080\r\n` +
            `Accept: application/json\r\n` +
            `Connection: keep-alive\r\n` +
            `\r\n`;

        // Simulate response
        const response =
            `HTTP/1.1 200 OK\r\n` +
            `Content-Type: application/json\r\n` +
            `Content-Length: 32\r\n` +
            `\r\n` +
            `{"id":${i},"status":"ok"}`;

        // Parse response - find body
        const parts = response.split("\r\n\r\n");
        if (parts.length === 2) {
            totalBytes += parts[1].length;
        }
        totalBytes += request.length + response.length;
    }

    const elapsed = performance.now() - start;
    console.error(`TIME_MS:${elapsed.toFixed(2)}`);
    console.log(totalBytes);
}

main();
