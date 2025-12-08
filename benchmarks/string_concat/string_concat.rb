n = ARGV[0] ? ARGV[0].to_i : 100000

result = ""
n.times do
  result += "a"
end

puts result.length
