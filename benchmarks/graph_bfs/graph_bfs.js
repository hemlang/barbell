function bfs(adj, n, start) {
    const visited = new Array(n).fill(false);
    const queue = [start];
    let head = 0;
    visited[start] = true;
    let count = 0;

    while (head < queue.length) {
        const node = queue[head++];
        count++;

        for (const neighbor of adj[node]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                queue.push(neighbor);
            }
        }
    }

    return count;
}

const n = parseInt(process.argv[2]) || 10000;

const start = performance.now();

const adj = [];
for (let i = 0; i < n; i++) {
    adj.push([]);
}

// Build a connected graph: each node connects to a few others
// Using LCG for deterministic pseudo-random edges
let seed = 12345n;
for (let i = 0; i < n; i++) {
    // Connect to next node (ensures connectivity)
    if (i + 1 < n) {
        adj[i].push(i + 1);
        adj[i + 1].push(i);
    }
    // Add some random edges
    for (let j = 0; j < 3; j++) {
        seed = (seed * 1103515245n + 12345n) % 4294967296n;
        const target = Number((seed >> 16n) & 0x7FFFn) % n;
        if (target !== i) {
            adj[i].push(target);
        }
    }
}

// Run BFS from node 0
const result = bfs(adj, n, 0);
const elapsed = performance.now() - start;
console.error(`TIME_MS:${elapsed.toFixed(2)}`);
console.log(result);
