// JSON deserialization benchmark - JavaScript
const n = parseInt(process.argv[2]) || 100000;

let totalId = 0;

for (let i = 0; i < n; i++) {
    const jsonStr = `{"id":${i},"name":"benchmark_test","value":${3.14159 + (i % 100) * 0.001},"active":true,"tags":[1,2,3,4,5]}`;
    const record = JSON.parse(jsonStr);
    totalId += record.id;
}

console.log(totalId);
