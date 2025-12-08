import sys

n = int(sys.argv[1]) if len(sys.argv) > 1 else 1000000

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

print(count)
