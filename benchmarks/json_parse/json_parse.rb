#!/usr/bin/env ruby
# JSON parse/serialize benchmark - Ruby implementation

require 'json'

TEST_JSON = <<~JSON
{
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
}
JSON

n = ARGV[0]&.to_i

if n.nil? || n == 0
  STDERR.puts "Usage: ruby json_parse.rb <iterations>"
  exit 1
end

total_len = 0

n.times do
  # Parse
  data = JSON.parse(TEST_JSON)
  # Serialize
  output = JSON.generate(data)
  total_len += output.length
end

puts total_len
