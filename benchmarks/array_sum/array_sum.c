#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int main(int argc, char *argv[]) {
    int n = argc > 1 ? atoi(argv[1]) : 10000000;

    struct timeval start, end;
    gettimeofday(&start, NULL);

    long *arr = malloc(n * sizeof(long));
    for (int i = 0; i < n; i++) {
        arr[i] = i;
    }

    long sum = 0;
    for (int i = 0; i < n; i++) {
        sum += arr[i];
    }

    gettimeofday(&end, NULL);
    double elapsed_ms = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_usec - start.tv_usec) / 1000.0;
    fprintf(stderr, "TIME_MS:%.2f\n", elapsed_ms);

    printf("%ld\n", sum);
    free(arr);
    return 0;
}
