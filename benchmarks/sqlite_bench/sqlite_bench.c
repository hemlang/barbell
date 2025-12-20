// SQLite benchmark - C
// Tests: create table, insert, query, update, delete

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

int main(int argc, char *argv[]) {
    int n = 1000;
    if (argc > 1) {
        n = atoi(argv[1]);
    }

    sqlite3 *db;
    sqlite3_stmt *stmt;
    char sql[256];
    int rc;
    long long total_score = 0;

    // Use in-memory database for benchmarking
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    // Create table
    rc = sqlite3_exec(db, "CREATE TABLE users (id INTEGER PRIMARY KEY, name TEXT, email TEXT, score INTEGER)", NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    // Begin transaction for inserts
    sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, NULL);

    // Prepare insert statement
    rc = sqlite3_prepare_v2(db, "INSERT INTO users (name, email, score) VALUES (?, ?, ?)", -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Prepare error: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    // Insert N rows
    char name[32], email[64];
    for (int i = 0; i < n; i++) {
        snprintf(name, sizeof(name), "user%d", i);
        snprintf(email, sizeof(email), "user%d@example.com", i);

        sqlite3_bind_text(stmt, 1, name, -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, email, -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 3, i * 10);

        sqlite3_step(stmt);
        sqlite3_reset(stmt);
    }
    sqlite3_finalize(stmt);

    // Commit transaction
    sqlite3_exec(db, "COMMIT", NULL, NULL, NULL);

    // Query all rows
    rc = sqlite3_prepare_v2(db, "SELECT * FROM users", -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Query prepare error: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        total_score += sqlite3_column_int(stmt, 3);
    }
    sqlite3_finalize(stmt);

    // Begin transaction for updates
    sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, NULL);

    // Prepare update statement
    rc = sqlite3_prepare_v2(db, "UPDATE users SET score = score + 1 WHERE id = ?", -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Update prepare error: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    // Update all rows
    for (int i = 0; i < n; i++) {
        sqlite3_bind_int(stmt, 1, i + 1);
        sqlite3_step(stmt);
        sqlite3_reset(stmt);
    }
    sqlite3_finalize(stmt);

    // Commit transaction
    sqlite3_exec(db, "COMMIT", NULL, NULL, NULL);

    // Delete all rows
    sqlite3_exec(db, "DELETE FROM users", NULL, NULL, NULL);

    sqlite3_close(db);

    printf("%lld\n", total_score);
    return 0;
}
