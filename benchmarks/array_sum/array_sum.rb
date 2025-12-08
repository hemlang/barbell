n = ARGV[0] ? ARGV[0].to_i : 10000000

arr = (0...n).to_a
total = arr.sum

puts total
