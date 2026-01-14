import sys
import time
from collections import deque

def bfs(adj, n, start):
    visited = [False] * n
    queue = deque([start])
    visited[start] = True
    count = 0

    while queue:
        node = queue.popleft()
        count += 1

        for neighbor in adj[node]:
            if not visited[neighbor]:
                visited[neighbor] = True
                queue.append(neighbor)

    return count

n = int(sys.argv[1]) if len(sys.argv) > 1 else 10000

start = time.perf_counter()

adj = [[] for _ in range(n)]

# Build a connected graph: each node connects to a few others
# Using LCG for deterministic pseudo-random edges
seed = 12345
for i in range(n):
    # Connect to next node (ensures connectivity)
    if i + 1 < n:
        adj[i].append(i + 1)
        adj[i + 1].append(i)
    # Add some random edges
    for j in range(3):
        seed = (seed * 1103515245 + 12345) & 0xFFFFFFFF
        target = ((seed >> 16) & 0x7FFF) % n
        if target != i:
            adj[i].append(target)

# Run BFS from node 0
result = bfs(adj, n, 0)
elapsed = (time.perf_counter() - start) * 1000
print(f"TIME_MS:{elapsed:.2f}", file=sys.stderr)
print(result)
