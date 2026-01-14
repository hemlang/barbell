n = ARGV[0] ? ARGV[0].to_i : 1000000

start = Process.clock_gettime(Process::CLOCK_MONOTONIC)

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

elapsed = (Process.clock_gettime(Process::CLOCK_MONOTONIC) - start) * 1000
STDERR.puts "TIME_MS:#{format('%.2f', elapsed)}"
puts count
