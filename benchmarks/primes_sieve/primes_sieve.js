const n = parseInt(process.argv[2]) || 1000000;

const sieve = new Array(n + 1).fill(false);

for (let i = 2; i * i <= n; i++) {
    if (!sieve[i]) {
        for (let j = i * i; j <= n; j += i) {
            sieve[j] = true;
        }
    }
}

let count = 0;
for (let i = 2; i <= n; i++) {
    if (!sieve[i]) count++;
}

console.log(count);
