#!/usr/bin/env ruby
# HTTP throughput benchmark - Ruby implementation

require 'socket'

port = ARGV[0]&.to_i

if port.nil? || port == 0
  STDERR.puts "Usage: ruby http_throughput.rb <port>"
  exit 1
end

response_body = '{"message":"Hello World!"}'
response = "HTTP/1.1 200 OK\r\n" \
           "Content-Type: application/json\r\n" \
           "Content-Length: #{response_body.length}\r\n" \
           "Connection: close\r\n" \
           "\r\n" \
           "#{response_body}"

server = TCPServer.new('0.0.0.0', port)
server.setsockopt(Socket::SOL_SOCKET, Socket::SO_REUSEADDR, true)

running = true
trap('TERM') { running = false; server.close rescue nil }
trap('INT') { running = false; server.close rescue nil }

# Signal ready
puts "READY #{port}"
STDOUT.flush

while running
  begin
    client = server.accept

    # Read request line
    request = client.gets
    next if request.nil?

    # Read remaining headers
    while (line = client.gets) && line != "\r\n"
      # Skip headers
    end

    if request.start_with?('GET ')
      client.write(response)
    end

    client.close rescue nil
  rescue IOError, Errno::EBADF
    break
  rescue => e
    # Connection errors are expected during shutdown
    break unless running
  end
end

server.close rescue nil
