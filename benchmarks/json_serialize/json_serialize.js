// JSON serialization benchmark - JavaScript
const n = parseInt(process.argv[2]) || 100000;

// Create a record to serialize
const record = {
    id: 12345,
    name: "benchmark_test",
    value: 3.14159265359,
    active: true,
    tags: [1, 2, 3, 4, 5]
};

let totalLen = 0;

for (let i = 0; i < n; i++) {
    record.id = i;
    record.value = 3.14159 + (i % 100) * 0.001;
    const jsonStr = JSON.stringify(record);
    totalLen += jsonStr.length;
}

console.log(totalLen);
