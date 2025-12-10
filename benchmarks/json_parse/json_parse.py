#!/usr/bin/env python3
# JSON parse/serialize benchmark - Python implementation

import sys
import json

TEST_JSON = '''{
    "name": "John Doe",
    "age": 30,
    "active": true,
    "balance": 1234.56,
    "address": {
        "street": "123 Main St",
        "city": "Springfield",
        "zip": "12345"
    },
    "tags": ["developer", "golang", "python", "rust"],
    "scores": [95, 87, 92, 88, 91],
    "metadata": null
}'''

def main():
    if len(sys.argv) < 2:
        print(f"Usage: {sys.argv[0]} <iterations>", file=sys.stderr)
        sys.exit(1)

    n = int(sys.argv[1])
    total_len = 0

    for _ in range(n):
        # Parse
        data = json.loads(TEST_JSON)
        # Serialize
        output = json.dumps(data)
        total_len += len(output)

    print(total_len)

if __name__ == '__main__':
    main()
