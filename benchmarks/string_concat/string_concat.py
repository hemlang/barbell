import sys

n = int(sys.argv[1]) if len(sys.argv) > 1 else 100000

result = ""
for i in range(n):
    result += "a"

print(len(result))
