#!/usr/bin/env node
// JSON parse/serialize benchmark - JavaScript implementation

const TEST_JSON = `{
    "name": "John Doe",
    "age": 30,
    "active": true,
    "balance": 1234.56,
    "address": {
        "street": "123 Main St",
        "city": "Springfield",
        "zip": "12345"
    },
    "tags": ["developer", "golang", "python", "rust"],
    "scores": [95, 87, 92, 88, 91],
    "metadata": null
}`;

const n = parseInt(process.argv[2]);

if (!n) {
    console.error('Usage: node json_parse.js <iterations>');
    process.exit(1);
}

let totalLen = 0;

for (let i = 0; i < n; i++) {
    // Parse
    const data = JSON.parse(TEST_JSON);
    // Serialize
    const output = JSON.stringify(data);
    totalLen += output.length;
}

console.log(totalLen);
