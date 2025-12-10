# JSON deserialization benchmark - Python
import sys
import json

n = int(sys.argv[1]) if len(sys.argv) > 1 else 100000

# JSON template
json_template = '{{"id":{},"name":"benchmark_test","value":{},"active":true,"tags":[1,2,3,4,5]}}'

total_id = 0

for i in range(n):
    json_str = json_template.format(i, 3.14159 + (i % 100) * 0.001)
    record = json.loads(json_str)
    total_id += record["id"]

print(total_id)
