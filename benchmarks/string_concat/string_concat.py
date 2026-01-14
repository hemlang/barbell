import sys
import time

n = int(sys.argv[1]) if len(sys.argv) > 1 else 100000

start = time.perf_counter()
result = ""
for i in range(n):
    result += "a"
elapsed = (time.perf_counter() - start) * 1000
print(f"TIME_MS:{elapsed:.2f}", file=sys.stderr)

print(len(result))
