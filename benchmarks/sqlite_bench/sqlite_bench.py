#!/usr/bin/env python3
# SQLite benchmark - Python
# Tests: create table, insert, query, update, delete

import sqlite3
import sys

n = 1000
if len(sys.argv) > 1:
    n = int(sys.argv[1])

# Use in-memory database for benchmarking
conn = sqlite3.connect(":memory:")
cursor = conn.cursor()

# Create table
cursor.execute("CREATE TABLE users (id INTEGER PRIMARY KEY, name TEXT, email TEXT, score INTEGER)")

# Insert N rows
for i in range(n):
    cursor.execute("INSERT INTO users (name, email, score) VALUES (?, ?, ?)",
                   (f"user{i}", f"user{i}@example.com", i * 10))
conn.commit()

# Query all rows
cursor.execute("SELECT * FROM users")
rows = cursor.fetchall()
total_score = sum(row[3] for row in rows)

# Update all rows
for i in range(n):
    cursor.execute("UPDATE users SET score = score + 1 WHERE id = ?", (i + 1,))
conn.commit()

# Delete all rows
cursor.execute("DELETE FROM users")
conn.commit()

conn.close()

print(total_score)
