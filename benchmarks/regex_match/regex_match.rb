iterations = ARGV[0] ? ARGV[0].to_i : 10000

# Generate test text with patterns to match
base = "The quick brown fox jumps over the lazy dog. " \
       "Email: user123@example.com Phone: 555-1234 " \
       "Date: 2024-01-15 Price: $99.99 " \
       "URL: https://www.example.com/path?query=value " \
       "IP: 192.168.1.1 Code: ABC-123-XYZ\n"

text = base * iterations

# Various regex patterns to test different matching scenarios
patterns = [
  /[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}/,  # Email
  /[0-9]{3}-[0-9]{4}/,                                # Phone
  /[0-9]{4}-[0-9]{2}-[0-9]{2}/,                       # Date
  /\$[0-9]+\.[0-9]{2}/,                               # Price
  /https?:\/\/[a-zA-Z0-9.-]+\/[a-zA-Z0-9\/?=&._-]*/,  # URL
  /[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}/,   # IP
  /[A-Z]{3}-[0-9]{3}-[A-Z]{3}/,                       # Code
  /\b[a-z]{4,}\b/                                     # 4+ letter words
]

total_matches = 0
patterns.each do |pattern|
  total_matches += text.scan(pattern).length
end

puts total_matches
