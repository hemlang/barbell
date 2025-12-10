#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int *neighbors;
    int count;
    int capacity;
} AdjList;

typedef struct {
    int *data;
    int head;
    int tail;
    int size;
} Queue;

Queue* queue_new(int capacity) {
    Queue *q = malloc(sizeof(Queue));
    q->data = malloc(capacity * sizeof(int));
    q->head = 0;
    q->tail = 0;
    q->size = capacity;
    return q;
}

void queue_push(Queue *q, int val) {
    q->data[q->tail] = val;
    q->tail = (q->tail + 1) % q->size;
}

int queue_pop(Queue *q) {
    int val = q->data[q->head];
    q->head = (q->head + 1) % q->size;
    return val;
}

int queue_empty(Queue *q) {
    return q->head == q->tail;
}

void queue_free(Queue *q) {
    free(q->data);
    free(q);
}

void add_edge(AdjList *adj, int from, int to) {
    if (adj[from].count >= adj[from].capacity) {
        adj[from].capacity = adj[from].capacity == 0 ? 4 : adj[from].capacity * 2;
        adj[from].neighbors = realloc(adj[from].neighbors, adj[from].capacity * sizeof(int));
    }
    adj[from].neighbors[adj[from].count++] = to;
}

int bfs(AdjList *adj, int n, int start) {
    char *visited = calloc(n, sizeof(char));
    Queue *q = queue_new(n);

    visited[start] = 1;
    queue_push(q, start);
    int count = 0;

    while (!queue_empty(q)) {
        int node = queue_pop(q);
        count++;

        for (int i = 0; i < adj[node].count; i++) {
            int neighbor = adj[node].neighbors[i];
            if (!visited[neighbor]) {
                visited[neighbor] = 1;
                queue_push(q, neighbor);
            }
        }
    }

    free(visited);
    queue_free(q);
    return count;
}

int main(int argc, char *argv[]) {
    int n = argc > 1 ? atoi(argv[1]) : 10000;

    AdjList *adj = calloc(n, sizeof(AdjList));

    // Build a connected graph: each node connects to a few others
    // Using LCG for deterministic pseudo-random edges
    unsigned int seed = 12345;
    for (int i = 0; i < n; i++) {
        // Connect to next node (ensures connectivity)
        if (i + 1 < n) {
            add_edge(adj, i, i + 1);
            add_edge(adj, i + 1, i);
        }
        // Add some random edges
        for (int j = 0; j < 3; j++) {
            seed = seed * 1103515245 + 12345;
            int target = ((seed >> 16) & 0x7FFF) % n;
            if (target != i) {
                add_edge(adj, i, target);
            }
        }
    }

    // Run BFS from node 0
    int visited_count = bfs(adj, n, 0);
    printf("%d\n", visited_count);

    // Cleanup
    for (int i = 0; i < n; i++) {
        free(adj[i].neighbors);
    }
    free(adj);

    return 0;
}
