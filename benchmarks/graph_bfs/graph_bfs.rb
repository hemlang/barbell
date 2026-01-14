def bfs(adj, n, start)
  visited = Array.new(n, false)
  queue = [start]
  visited[start] = true
  count = 0

  until queue.empty?
    node = queue.shift
    count += 1

    adj[node].each do |neighbor|
      unless visited[neighbor]
        visited[neighbor] = true
        queue.push(neighbor)
      end
    end
  end

  count
end

n = ARGV[0] ? ARGV[0].to_i : 10000

start = Process.clock_gettime(Process::CLOCK_MONOTONIC)

adj = Array.new(n) { [] }

# Build a connected graph: each node connects to a few others
# Using LCG for deterministic pseudo-random edges
seed = 12345
n.times do |i|
  # Connect to next node (ensures connectivity)
  if i + 1 < n
    adj[i].push(i + 1)
    adj[i + 1].push(i)
  end
  # Add some random edges
  3.times do
    seed = (seed * 1103515245 + 12345) & 0xFFFFFFFF
    target = ((seed >> 16) & 0x7FFF) % n
    adj[i].push(target) if target != i
  end
end

# Run BFS from node 0
result = bfs(adj, n, 0)

elapsed = (Process.clock_gettime(Process::CLOCK_MONOTONIC) - start) * 1000
STDERR.puts "TIME_MS:#{format('%.2f', elapsed)}"
puts result
