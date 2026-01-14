import sys
import time

n = int(sys.argv[1]) if len(sys.argv) > 1 else 1000000

start = time.perf_counter()

sieve = [False] * (n + 1)

i = 2
while i * i <= n:
    if not sieve[i]:
        j = i * i
        while j <= n:
            sieve[j] = True
            j += i
    i += 1

count = 0
for i in range(2, n + 1):
    if not sieve[i]:
        count += 1

elapsed = (time.perf_counter() - start) * 1000
print(f"TIME_MS:{elapsed:.2f}", file=sys.stderr)
print(count)
