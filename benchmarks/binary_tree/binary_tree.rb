class Node
  attr_accessor :value, :left, :right

  def initialize(value)
    @value = value
    @left = nil
    @right = nil
  end
end

def insert(root, value)
  return Node.new(value) if root.nil?

  if value < root.value
    root.left = insert(root.left, value)
  else
    root.right = insert(root.right, value)
  end
  root
end

def inorder_sum(root)
  return 0 if root.nil?
  inorder_sum(root.left) + root.value + inorder_sum(root.right)
end

n = ARGV[0] ? ARGV[0].to_i : 100000

root = nil

# Insert values using LCG for pseudo-random values
seed = 12345
n.times do
  seed = (seed * 1103515245 + 12345) & 0xFFFFFFFF
  value = (seed >> 16) & 0x7FFF
  root = insert(root, value)
end

# Sum all values via in-order traversal
puts inorder_sum(root)
