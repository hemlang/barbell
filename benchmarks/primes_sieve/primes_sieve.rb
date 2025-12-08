n = ARGV[0] ? ARGV[0].to_i : 1000000

sieve = Array.new(n + 1, false)

i = 2
while i * i <= n
  unless sieve[i]
    j = i * i
    while j <= n
      sieve[j] = true
      j += i
    end
  end
  i += 1
end

count = 0
(2..n).each do |i|
  count += 1 unless sieve[i]
end

puts count
