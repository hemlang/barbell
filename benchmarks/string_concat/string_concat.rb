n = ARGV[0] ? ARGV[0].to_i : 100000

start = Process.clock_gettime(Process::CLOCK_MONOTONIC)

result = ""
n.times do
  result += "a"
end

elapsed = (Process.clock_gettime(Process::CLOCK_MONOTONIC) - start) * 1000
STDERR.puts "TIME_MS:#{format('%.2f', elapsed)}"
puts result.length
