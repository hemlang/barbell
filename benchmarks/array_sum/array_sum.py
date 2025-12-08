import sys

n = int(sys.argv[1]) if len(sys.argv) > 1 else 10000000

arr = list(range(n))
total = sum(arr)

print(total)
