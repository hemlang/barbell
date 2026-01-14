const n = parseInt(process.argv[2]) || 100000;

const start = performance.now();

let result = "";
for (let i = 0; i < n; i++) {
    result += "a";
}

const elapsed = performance.now() - start;
console.error(`TIME_MS:${elapsed.toFixed(2)}`);
console.log(result.length);
