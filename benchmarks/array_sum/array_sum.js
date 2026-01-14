const n = parseInt(process.argv[2]) || 10000000;

const start = performance.now();

const arr = [];
for (let i = 0; i < n; i++) {
    arr.push(i);
}

let sum = 0;
for (let i = 0; i < n; i++) {
    sum += arr[i];
}

const elapsed = performance.now() - start;
console.error(`TIME_MS:${elapsed.toFixed(2)}`);
console.log(sum);
