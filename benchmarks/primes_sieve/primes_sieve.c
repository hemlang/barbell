#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

int main(int argc, char *argv[]) {
    int n = argc > 1 ? atoi(argv[1]) : 1000000;

    struct timeval start, end;
    gettimeofday(&start, NULL);

    char *sieve = calloc(n + 1, 1);

    for (int i = 2; i * i <= n; i++) {
        if (!sieve[i]) {
            for (int j = i * i; j <= n; j += i) {
                sieve[j] = 1;
            }
        }
    }

    int count = 0;
    for (int i = 2; i <= n; i++) {
        if (!sieve[i]) count++;
    }

    gettimeofday(&end, NULL);
    double elapsed_ms = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_usec - start.tv_usec) / 1000.0;
    fprintf(stderr, "TIME_MS:%.2f\n", elapsed_ms);

    printf("%d\n", count);
    free(sieve);
    return 0;
}
