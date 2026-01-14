class Node {
    constructor(value) {
        this.value = value;
        this.left = null;
        this.right = null;
    }
}

function insert(root, value) {
    if (root === null) {
        return new Node(value);
    }
    if (value < root.value) {
        root.left = insert(root.left, value);
    } else {
        root.right = insert(root.right, value);
    }
    return root;
}

function inorderSum(root) {
    if (root === null) return 0;
    return inorderSum(root.left) + root.value + inorderSum(root.right);
}

const n = parseInt(process.argv[2]) || 100000;

const start = performance.now();

let root = null;

// Insert values using LCG for pseudo-random values
let seed = 12345n;
for (let i = 0; i < n; i++) {
    seed = (seed * 1103515245n + 12345n) % 4294967296n;
    const value = Number((seed >> 16n) & 0x7FFFn);
    root = insert(root, value);
}

// Sum all values via in-order traversal
const result = inorderSum(root);
const elapsed = performance.now() - start;
console.error(`TIME_MS:${elapsed.toFixed(2)}`);
console.log(result);
