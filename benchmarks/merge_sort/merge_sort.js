function merge(arr, left, mid, right, temp) {
    let i = left, j = mid + 1, k = left;

    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
    }
    while (i <= mid) {
        temp[k++] = arr[i++];
    }
    while (j <= right) {
        temp[k++] = arr[j++];
    }
    for (let x = left; x <= right; x++) {
        arr[x] = temp[x];
    }
}

function mergeSort(arr, left, right, temp) {
    if (left < right) {
        const mid = left + Math.floor((right - left) / 2);
        mergeSort(arr, left, mid, temp);
        mergeSort(arr, mid + 1, right, temp);
        merge(arr, left, mid, right, temp);
    }
}

const n = parseInt(process.argv[2]) || 100000;

// Fill with descending values
const arr = [];
for (let i = n; i > 0; i--) {
    arr.push(i);
}
const temp = new Array(n).fill(0);

mergeSort(arr, 0, n - 1, temp);

// Print checksum
let sum = 0;
for (let i = 0; i < n; i++) {
    sum += arr[i];
}
console.log(sum);
