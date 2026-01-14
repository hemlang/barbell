// JSON serialization benchmark - C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

// Simple struct to serialize
typedef struct {
    int id;
    char name[64];
    double value;
    int active;
    int tags[5];
} Record;

// Manual JSON serialization (no external dependencies)
int serialize_record(Record *r, char *buf, size_t buf_size) {
    return snprintf(buf, buf_size,
        "{\"id\":%d,\"name\":\"%s\",\"value\":%.6f,\"active\":%s,\"tags\":[%d,%d,%d,%d,%d]}",
        r->id, r->name, r->value, r->active ? "true" : "false",
        r->tags[0], r->tags[1], r->tags[2], r->tags[3], r->tags[4]);
}

int main(int argc, char *argv[]) {
    int n = argc > 1 ? atoi(argv[1]) : 100000;

    struct timeval start, end;
    gettimeofday(&start, NULL);

    // Create a record to serialize
    Record rec = {
        .id = 12345,
        .name = "benchmark_test",
        .value = 3.14159265359,
        .active = 1,
        .tags = {1, 2, 3, 4, 5}
    };

    char buffer[256];
    long total_len = 0;

    for (int i = 0; i < n; i++) {
        rec.id = i;
        rec.value = 3.14159 + (i % 100) * 0.001;
        int len = serialize_record(&rec, buffer, sizeof(buffer));
        total_len += len;
    }

    gettimeofday(&end, NULL);
    double elapsed_ms = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_usec - start.tv_usec) / 1000.0;
    fprintf(stderr, "TIME_MS:%.2f\n", elapsed_ms);

    printf("%ld\n", total_len);
    return 0;
}
