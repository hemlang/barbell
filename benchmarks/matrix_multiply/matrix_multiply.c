#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int n = argc > 1 ? atoi(argv[1]) : 200;

    // Allocate matrices
    double *a = malloc(n * n * sizeof(double));
    double *b = malloc(n * n * sizeof(double));
    double *c = malloc(n * n * sizeof(double));

    // Initialize matrices
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            a[i * n + j] = (double)(i + j) / n;
            b[i * n + j] = (double)(i - j) / n;
            c[i * n + j] = 0.0;
        }
    }

    // Matrix multiply: C = A * B
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            double sum = 0.0;
            for (int k = 0; k < n; k++) {
                sum += a[i * n + k] * b[k * n + j];
            }
            c[i * n + j] = sum;
        }
    }

    // Compute checksum
    double checksum = 0.0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            checksum += c[i * n + j];
        }
    }
    printf("%.6f\n", checksum);

    free(a);
    free(b);
    free(c);
    return 0;
}
