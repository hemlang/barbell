import sys
import time

n = int(sys.argv[1]) if len(sys.argv) > 1 else 10000000

start = time.perf_counter()
arr = list(range(n))
total = sum(arr)
elapsed = (time.perf_counter() - start) * 1000
print(f"TIME_MS:{elapsed:.2f}", file=sys.stderr)

print(total)
