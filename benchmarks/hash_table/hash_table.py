import sys

n = int(sys.argv[1]) if len(sys.argv) > 1 else 100000

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

print(checksum)
