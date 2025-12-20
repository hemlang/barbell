// SQLite benchmark - JavaScript (Node.js with better-sqlite3)
// Tests: create table, insert, query, update, delete

const Database = require('better-sqlite3');

const n = parseInt(process.argv[2]) || 1000;

// Use in-memory database for benchmarking
const db = new Database(':memory:');

// Create table
db.exec('CREATE TABLE users (id INTEGER PRIMARY KEY, name TEXT, email TEXT, score INTEGER)');

// Insert N rows using transaction for performance
const insert = db.prepare('INSERT INTO users (name, email, score) VALUES (?, ?, ?)');
const insertMany = db.transaction((items) => {
    for (const item of items) insert.run(item.name, item.email, item.score);
});

const items = [];
for (let i = 0; i < n; i++) {
    items.push({ name: `user${i}`, email: `user${i}@example.com`, score: i * 10 });
}
insertMany(items);

// Query all rows
const rows = db.prepare('SELECT * FROM users').all();
let totalScore = 0;
for (const row of rows) {
    totalScore += row.score;
}

// Update all rows
const update = db.prepare('UPDATE users SET score = score + 1 WHERE id = ?');
const updateMany = db.transaction(() => {
    for (let i = 0; i < n; i++) {
        update.run(i + 1);
    }
});
updateMany();

// Delete all rows
db.exec('DELETE FROM users');

db.close();

console.log(totalScore);
