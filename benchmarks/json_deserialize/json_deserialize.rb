# JSON deserialization benchmark - Ruby
require 'json'

n = ARGV[0] ? ARGV[0].to_i : 100000

total_id = 0

n.times do |i|
    json_str = "{\"id\":#{i},\"name\":\"benchmark_test\",\"value\":#{3.14159 + (i % 100) * 0.001},\"active\":true,\"tags\":[1,2,3,4,5]}"
    record = JSON.parse(json_str)
    total_id += record["id"]
end

puts total_id
