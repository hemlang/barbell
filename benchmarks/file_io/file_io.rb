require 'tempfile'

n = ARGV[0] ? ARGV[0].to_i : 1000000

# Create temp file
file = Tempfile.new('barbell_file_io_')
filename = file.path
file.close

begin
  # Write n bytes
  data = (0...n).map { |i| (i % 256).chr }.join.b
  File.binwrite(filename, data)

  # Read back and sum
  read_data = File.binread(filename)
  total = read_data.bytes.sum

  puts total
ensure
  File.unlink(filename) if File.exist?(filename)
end
