#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node *left;
    struct Node *right;
} Node;

Node* new_node(int value) {
    Node *n = malloc(sizeof(Node));
    n->value = value;
    n->left = NULL;
    n->right = NULL;
    return n;
}

Node* insert(Node *root, int value) {
    if (root == NULL) {
        return new_node(value);
    }
    if (value < root->value) {
        root->left = insert(root->left, value);
    } else {
        root->right = insert(root->right, value);
    }
    return root;
}

long inorder_sum(Node *root) {
    if (root == NULL) return 0;
    return inorder_sum(root->left) + root->value + inorder_sum(root->right);
}

void free_tree(Node *root) {
    if (root == NULL) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

int main(int argc, char *argv[]) {
    int n = argc > 1 ? atoi(argv[1]) : 100000;

    Node *root = NULL;

    // Insert values in a pattern that creates a balanced-ish tree
    // Using a simple LCG to generate pseudo-random values
    unsigned int seed = 12345;
    for (int i = 0; i < n; i++) {
        seed = seed * 1103515245 + 12345;
        int value = (seed >> 16) & 0x7FFF;
        root = insert(root, value);
    }

    // Sum all values via in-order traversal
    long sum = inorder_sum(root);
    printf("%ld\n", sum);

    free_tree(root);
    return 0;
}
