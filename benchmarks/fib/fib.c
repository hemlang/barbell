#include <stdio.h>
#include <stdlib.h>

long fib(int n) {
    if (n <= 1) return n;
    return fib(n - 1) + fib(n - 2);
}

int main(int argc, char *argv[]) {
    int n = argc > 1 ? atoi(argv[1]) : 35;
    printf("%ld\n", fib(n));
    return 0;
}
