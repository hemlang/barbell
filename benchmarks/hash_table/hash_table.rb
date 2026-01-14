n = ARGV[0] ? ARGV[0].to_i : 100000

start = Process.clock_gettime(Process::CLOCK_MONOTONIC)

ht = {}
checksum = 0

# Insert n items
n.times do |i|
  ht["key_#{i}"] = i * 2
end

# Lookup all items
n.times do |i|
  key = "key_#{i}"
  checksum += ht[key] if ht.key?(key)
end

# Delete half the items
(0...n).step(2) do |i|
  ht.delete("key_#{i}")
end

# Lookup remaining items
n.times do |i|
  key = "key_#{i}"
  checksum += ht[key] if ht.key?(key)
end

elapsed = (Process.clock_gettime(Process::CLOCK_MONOTONIC) - start) * 1000
STDERR.puts "TIME_MS:#{format('%.2f', elapsed)}"
puts checksum
