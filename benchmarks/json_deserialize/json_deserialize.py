# JSON deserialization benchmark - Python
import sys
import json
import time

n = int(sys.argv[1]) if len(sys.argv) > 1 else 100000

start = time.perf_counter()

# JSON template
json_template = '{{"id":{},"name":"benchmark_test","value":{},"active":true,"tags":[1,2,3,4,5]}}'

total_id = 0

for i in range(n):
    json_str = json_template.format(i, 3.14159 + (i % 100) * 0.001)
    record = json.loads(json_str)
    total_id += record["id"]

elapsed = (time.perf_counter() - start) * 1000
print(f"TIME_MS:{elapsed:.2f}", file=sys.stderr)
print(total_id)
