import sys

n = int(sys.argv[1]) if len(sys.argv) > 1 else 200

# Initialize matrices
a = [[float(i + j) / n for j in range(n)] for i in range(n)]
b = [[float(i - j) / n for j in range(n)] for i in range(n)]
c = [[0.0] * n for _ in range(n)]

# Matrix multiply: C = A * B
for i in range(n):
    for j in range(n):
        s = 0.0
        for k in range(n):
            s += a[i][k] * b[k][j]
        c[i][j] = s

# Compute checksum
checksum = sum(sum(row) for row in c)
print(f"{checksum:.6f}")
