# Word count benchmark - Python
# Count words and lines in large generated text (classic MapReduce task)

import sys

n = int(sys.argv[1]) if len(sys.argv) > 1 else 100000

phrase = "the quick brown fox jumps over the lazy dog\n"

# Generate text: repeat phrase n times
text = phrase * n

# Count words and lines
words = len(text.split())
lines = text.count('\n')

print(f"{words} {lines}")
