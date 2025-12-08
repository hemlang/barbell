#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int n = argc > 1 ? atoi(argv[1]) : 1000000;

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

    printf("%d\n", count);
    free(sieve);
    return 0;
}
