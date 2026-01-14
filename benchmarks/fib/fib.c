#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

long fib(int n) {
    if (n <= 1) return n;
    return fib(n - 1) + fib(n - 2);
}

int main(int argc, char *argv[]) {
    int n = argc > 1 ? atoi(argv[1]) : 35;

    struct timeval start, end;
    gettimeofday(&start, NULL);

    long result = fib(n);

    gettimeofday(&end, NULL);
    double elapsed_ms = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_usec - start.tv_usec) / 1000.0;
    fprintf(stderr, "TIME_MS:%.2f\n", elapsed_ms);

    printf("%ld\n", result);
    return 0;
}
