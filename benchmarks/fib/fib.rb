def fib(n)
  return n if n <= 1
  fib(n - 1) + fib(n - 2)
end

n = ARGV[0] ? ARGV[0].to_i : 35
puts fib(n)
