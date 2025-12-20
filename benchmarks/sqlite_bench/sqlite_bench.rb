#!/usr/bin/env ruby
# SQLite benchmark - Ruby
# Tests: create table, insert, query, update, delete

require 'sqlite3'

n = (ARGV[0] || 1000).to_i

# Use in-memory database for benchmarking
db = SQLite3::Database.new(':memory:')

# Create table
db.execute('CREATE TABLE users (id INTEGER PRIMARY KEY, name TEXT, email TEXT, score INTEGER)')

# Insert N rows using transaction for performance
db.transaction do
  n.times do |i|
    db.execute('INSERT INTO users (name, email, score) VALUES (?, ?, ?)',
               ["user#{i}", "user#{i}@example.com", i * 10])
  end
end

# Query all rows
rows = db.execute('SELECT * FROM users')
total_score = rows.sum { |row| row[3] }

# Update all rows
db.transaction do
  n.times do |i|
    db.execute('UPDATE users SET score = score + 1 WHERE id = ?', [i + 1])
  end
end

# Delete all rows
db.execute('DELETE FROM users')

db.close

puts total_score
