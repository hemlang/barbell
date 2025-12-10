# JSON serialization benchmark - Python
import sys
import json

n = int(sys.argv[1]) if len(sys.argv) > 1 else 100000

# Create a record to serialize
record = {
    "id": 12345,
    "name": "benchmark_test",
    "value": 3.14159265359,
    "active": True,
    "tags": [1, 2, 3, 4, 5]
}

total_len = 0

for i in range(n):
    record["id"] = i
    record["value"] = 3.14159 + (i % 100) * 0.001
    json_str = json.dumps(record)
    total_len += len(json_str)

print(total_len)
