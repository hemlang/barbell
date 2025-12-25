const n = parseInt(process.argv[2]) || 200;

// Initialize matrices
const a = [];
const b = [];
const c = [];

for (let i = 0; i < n; i++) {
    a[i] = [];
    b[i] = [];
    c[i] = [];
    for (let j = 0; j < n; j++) {
        a[i][j] = (i + j) / n;
        b[i][j] = (i - j) / n;
        c[i][j] = 0.0;
    }
}

// Matrix multiply: C = A * B
for (let i = 0; i < n; i++) {
    for (let j = 0; j < n; j++) {
        let sum = 0.0;
        for (let k = 0; k < n; k++) {
            sum += a[i][k] * b[k][j];
        }
        c[i][j] = sum;
    }
}

// Compute checksum
let checksum = 0.0;
for (let i = 0; i < n; i++) {
    for (let j = 0; j < n; j++) {
        checksum += c[i][j];
    }
}
console.log(checksum.toFixed(6));
