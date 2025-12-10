#!/usr/bin/env python3
# Unicode normalization benchmark
import sys
import unicodedata

def main():
    iterations = int(sys.argv[1]) if len(sys.argv) > 1 else 100000

    # Test string with mixed NFD and NFC characters
    # NFD form: base characters followed by combining characters
    test_nfd = "cafe\u0301 na\u00EFve re\u0301sume\u0301 coo\u0308perate sen\u0303or"
    # NFC form: precomposed characters
    test_nfc = "caf\u00E9 na\u00EFve r\u00E9sum\u00E9 co\u00F6perate se\u00F1or"

    total_len = 0

    for _ in range(iterations):
        # NFC: compose combining sequences
        normalized1 = unicodedata.normalize('NFC', test_nfd)

        # NFD: decompose precomposed characters
        normalized2 = unicodedata.normalize('NFD', test_nfc)

        total_len += len(normalized1) + len(normalized2)

    print(total_len)

if __name__ == "__main__":
    main()
