#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int n = argc > 1 ? atoi(argv[1]) : 10000000;

    long *arr = malloc(n * sizeof(long));
    for (int i = 0; i < n; i++) {
        arr[i] = i;
    }

    long sum = 0;
    for (int i = 0; i < n; i++) {
        sum += arr[i];
    }

    printf("%ld\n", sum);
    free(arr);
    return 0;
}
