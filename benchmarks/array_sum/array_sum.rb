n = ARGV[0] ? ARGV[0].to_i : 10000000

start = Process.clock_gettime(Process::CLOCK_MONOTONIC)

arr = (0...n).to_a
total = arr.sum

elapsed = (Process.clock_gettime(Process::CLOCK_MONOTONIC) - start) * 1000
STDERR.puts "TIME_MS:#{format('%.2f', elapsed)}"
puts total
