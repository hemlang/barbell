#!/usr/bin/env ruby
# Unicode normalization benchmark

iterations = ARGV[0] ? ARGV[0].to_i : 100000

# Test string with mixed NFD and NFC characters
# NFD form: base characters followed by combining characters
test_nfd = "cafe\u0301 na\u00EFve re\u0301sume\u0301 coo\u0308perate sen\u0303or"
# NFC form: precomposed characters
test_nfc = "caf\u00E9 na\u00EFve r\u00E9sum\u00E9 co\u00F6perate se\u00F1or"

total_len = 0

iterations.times do
  # NFC: compose combining sequences
  normalized1 = test_nfd.unicode_normalize(:nfc)

  # NFD: decompose precomposed characters
  normalized2 = test_nfc.unicode_normalize(:nfd)

  total_len += normalized1.length + normalized2.length
end

puts total_len
