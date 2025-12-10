import sys
import os
import tempfile

n = int(sys.argv[1]) if len(sys.argv) > 1 else 1000000

# Create temp file
fd, filename = tempfile.mkstemp(prefix='barbell_file_io_')
os.close(fd)

try:
    # Write n bytes
    data = bytes(i % 256 for i in range(n))
    with open(filename, 'wb') as f:
        f.write(data)

    # Read back and sum
    with open(filename, 'rb') as f:
        read_data = f.read()

    total = sum(read_data)
    print(total)
finally:
    os.unlink(filename)
