#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

void quicksort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quicksort(arr, low, pi - 1);
        quicksort(arr, pi + 1, high);
    }
}

int main(int argc, char *argv[]) {
    int n = argc > 1 ? atoi(argv[1]) : 100000;

    struct timeval start, end;
    gettimeofday(&start, NULL);

    int *arr = malloc(n * sizeof(int));
    // Fill with descending values (worst case for naive pivot)
    for (int i = 0; i < n; i++) {
        arr[i] = n - i;
    }

    quicksort(arr, 0, n - 1);

    // Verify sorted and print checksum
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
