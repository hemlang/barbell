import sys
import re

iterations = int(sys.argv[1]) if len(sys.argv) > 1 else 10000

# Generate test text with patterns to match
base = """The quick brown fox jumps over the lazy dog. \
Email: user123@example.com Phone: 555-1234 \
Date: 2024-01-15 Price: $99.99 \
URL: https://www.example.com/path?query=value \
IP: 192.168.1.1 Code: ABC-123-XYZ
"""

text = base * iterations

# Various regex patterns to test different matching scenarios
patterns = [
    r'[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}',  # Email
    r'[0-9]{3}-[0-9]{4}',                                # Phone
    r'[0-9]{4}-[0-9]{2}-[0-9]{2}',                       # Date
    r'\$[0-9]+\.[0-9]{2}',                               # Price
    r'https?://[a-zA-Z0-9.-]+/[a-zA-Z0-9/?=&._-]*',      # URL
    r'[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}',   # IP
    r'[A-Z]{3}-[0-9]{3}-[A-Z]{3}',                       # Code
    r'\b[a-z]{4,}\b'                                     # 4+ letter words
]

total_matches = 0
for pattern in patterns:
    total_matches += len(re.findall(pattern, text))

print(total_matches)
