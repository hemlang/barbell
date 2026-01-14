function fib(n) {
    if (n <= 1) return n;
    return fib(n - 1) + fib(n - 2);
}

const n = parseInt(process.argv[2]) || 35;

const start = performance.now();
const result = fib(n);
const elapsed = performance.now() - start;
console.error(`TIME_MS:${elapsed.toFixed(2)}`);
console.log(result);
