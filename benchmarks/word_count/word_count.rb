# Word count benchmark - Ruby
# Count words and lines in large generated text (classic MapReduce task)

n = ARGV[0] ? ARGV[0].to_i : 100000

phrase = "the quick brown fox jumps over the lazy dog\n"

# Generate text: repeat phrase n times
text = phrase * n

# Count words and lines
words = text.split.length
lines = text.count("\n")

puts "#{words} #{lines}"
