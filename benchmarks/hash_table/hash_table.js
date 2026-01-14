const n = parseInt(process.argv[2]) || 100000;

const start = performance.now();

const ht = new Map();
let checksum = 0;

// Insert n items
for (let i = 0; i < n; i++) {
    ht.set(`key_${i}`, i * 2);
}

// Lookup all items
for (let i = 0; i < n; i++) {
    const key = `key_${i}`;
    if (ht.has(key)) {
        checksum += ht.get(key);
    }
}

// Delete half the items
for (let i = 0; i < n; i += 2) {
    ht.delete(`key_${i}`);
}

// Lookup remaining items
for (let i = 0; i < n; i++) {
    const key = `key_${i}`;
    if (ht.has(key)) {
        checksum += ht.get(key);
    }
}

const elapsed = performance.now() - start;
console.error(`TIME_MS:${elapsed.toFixed(2)}`);
console.log(checksum);
