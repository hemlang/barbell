def fib(n)
  return n if n <= 1
  fib(n - 1) + fib(n - 2)
end

n = ARGV[0] ? ARGV[0].to_i : 35

start = Process.clock_gettime(Process::CLOCK_MONOTONIC)
result = fib(n)
elapsed = (Process.clock_gettime(Process::CLOCK_MONOTONIC) - start) * 1000

STDERR.puts "TIME_MS:#{format('%.2f', elapsed)}"
puts result
