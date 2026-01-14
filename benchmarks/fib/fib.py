import sys
import time

def fib(n):
    if n <= 1:
        return n
    return fib(n - 1) + fib(n - 2)

n = int(sys.argv[1]) if len(sys.argv) > 1 else 35

start = time.perf_counter()
result = fib(n)
elapsed = (time.perf_counter() - start) * 1000
print(f"TIME_MS:{elapsed:.2f}", file=sys.stderr)
print(result)
