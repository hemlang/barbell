const fs = require('fs');
const os = require('os');
const path = require('path');

const n = parseInt(process.argv[2]) || 1000000;

// Create temp file
const filename = path.join(os.tmpdir(), `barbell_file_io_${process.pid}_${Date.now()}`);

// Write n bytes
const buf = Buffer.alloc(n);
for (let i = 0; i < n; i++) {
    buf[i] = i % 256;
}
fs.writeFileSync(filename, buf);

// Read back and sum
const readBuf = fs.readFileSync(filename);
let sum = 0;
for (let i = 0; i < readBuf.length; i++) {
    sum += readBuf[i];
}

// Cleanup
fs.unlinkSync(filename);

console.log(sum);
