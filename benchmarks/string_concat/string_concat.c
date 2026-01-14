#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

int main(int argc, char *argv[]) {
    int n = argc > 1 ? atoi(argv[1]) : 100000;

    struct timeval start, end;
    gettimeofday(&start, NULL);

    // Pre-allocate to be fair (C would normally use realloc)
    size_t capacity = n * 2;
    char *result = malloc(capacity);
    result[0] = '\0';
    size_t len = 0;

    for (int i = 0; i < n; i++) {
        result[len++] = 'a';
        result[len] = '\0';
    }

    gettimeofday(&end, NULL);
    double elapsed_ms = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_usec - start.tv_usec) / 1000.0;
    fprintf(stderr, "TIME_MS:%.2f\n", elapsed_ms);

    printf("%zu\n", len);
    free(result);
    return 0;
}
