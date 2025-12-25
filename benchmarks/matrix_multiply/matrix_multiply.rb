n = ARGV[0] ? ARGV[0].to_i : 200

# Initialize matrices
a = Array.new(n) { |i| Array.new(n) { |j| (i + j).to_f / n } }
b = Array.new(n) { |i| Array.new(n) { |j| (i - j).to_f / n } }
c = Array.new(n) { Array.new(n, 0.0) }

# Matrix multiply: C = A * B
n.times do |i|
  n.times do |j|
    sum = 0.0
    n.times do |k|
      sum += a[i][k] * b[k][j]
    end
    c[i][j] = sum
  end
end

# Compute checksum
checksum = c.flatten.sum
puts format("%.6f", checksum)
