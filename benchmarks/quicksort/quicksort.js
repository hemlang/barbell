function partition(arr, low, high) {
    const pivot = arr[high];
    let i = low - 1;
    for (let j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            [arr[i], arr[j]] = [arr[j], arr[i]];
        }
    }
    [arr[i + 1], arr[high]] = [arr[high], arr[i + 1]];
    return i + 1;
}

function quicksort(arr, low, high) {
    if (low < high) {
        const pi = partition(arr, low, high);
        quicksort(arr, low, pi - 1);
        quicksort(arr, pi + 1, high);
    }
}

const n = parseInt(process.argv[2]) || 100000;

const start = performance.now();

// Fill with descending values
const arr = [];
for (let i = n; i > 0; i--) {
    arr.push(i);
}

quicksort(arr, 0, n - 1);

// Print checksum
let sum = 0;
for (let i = 0; i < n; i++) {
    sum += arr[i];
}

const elapsed = performance.now() - start;
console.error(`TIME_MS:${elapsed.toFixed(2)}`);
console.log(sum);
