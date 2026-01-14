import sys
import time
sys.setrecursionlimit(50000)

class Node:
    __slots__ = ['value', 'left', 'right']
    def __init__(self, value):
        self.value = value
        self.left = None
        self.right = None

def insert(root, value):
    if root is None:
        return Node(value)
    if value < root.value:
        root.left = insert(root.left, value)
    else:
        root.right = insert(root.right, value)
    return root

def inorder_sum(root):
    if root is None:
        return 0
    return inorder_sum(root.left) + root.value + inorder_sum(root.right)

n = int(sys.argv[1]) if len(sys.argv) > 1 else 100000

start = time.perf_counter()

root = None

# Insert values using LCG for pseudo-random values
seed = 12345
for i in range(n):
    seed = (seed * 1103515245 + 12345) & 0xFFFFFFFF
    value = (seed >> 16) & 0x7FFF
    root = insert(root, value)

# Sum all values via in-order traversal
result = inorder_sum(root)
elapsed = (time.perf_counter() - start) * 1000
print(f"TIME_MS:{elapsed:.2f}", file=sys.stderr)
print(result)
