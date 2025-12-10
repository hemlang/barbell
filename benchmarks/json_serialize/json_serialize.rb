# JSON serialization benchmark - Ruby
require 'json'

n = ARGV[0] ? ARGV[0].to_i : 100000

# Create a record to serialize
record = {
    "id" => 12345,
    "name" => "benchmark_test",
    "value" => 3.14159265359,
    "active" => true,
    "tags" => [1, 2, 3, 4, 5]
}

total_len = 0

n.times do |i|
    record["id"] = i
    record["value"] = 3.14159 + (i % 100) * 0.001
    json_str = JSON.generate(record)
    total_len += json_str.length
end

puts total_len
