import sys
import time

n = int(sys.argv[1]) if len(sys.argv) > 1 else 100000

start = time.perf_counter()

ht = {}
checksum = 0

# Insert n items
for i in range(n):
    ht[f"key_{i}"] = i * 2

# Lookup all items
for i in range(n):
    key = f"key_{i}"
    if key in ht:
        checksum += ht[key]

# Delete half the items
for i in range(0, n, 2):
    del ht[f"key_{i}"]

# Lookup remaining items
for i in range(n):
    key = f"key_{i}"
    if key in ht:
        checksum += ht[key]

elapsed = (time.perf_counter() - start) * 1000
print(f"TIME_MS:{elapsed:.2f}", file=sys.stderr)
print(checksum)
