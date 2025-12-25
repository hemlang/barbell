#include <stdio.h>
#include <stdlib.h>

void merge(int arr[], int left, int mid, int right, int temp[]) {
    int i = left, j = mid + 1, k = left;

    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
    }
    while (i <= mid) {
        temp[k++] = arr[i++];
    }
    while (j <= right) {
        temp[k++] = arr[j++];
    }
    for (i = left; i <= right; i++) {
        arr[i] = temp[i];
    }
}

void merge_sort(int arr[], int left, int right, int temp[]) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        merge_sort(arr, left, mid, temp);
        merge_sort(arr, mid + 1, right, temp);
        merge(arr, left, mid, right, temp);
    }
}

int main(int argc, char *argv[]) {
    int n = argc > 1 ? atoi(argv[1]) : 100000;

    int *arr = malloc(n * sizeof(int));
    int *temp = malloc(n * sizeof(int));

    // Fill with descending values
    for (int i = 0; i < n; i++) {
        arr[i] = n - i;
    }

    merge_sort(arr, 0, n - 1, temp);

    // Print checksum
    long sum = 0;
    for (int i = 0; i < n; i++) {
        sum += arr[i];
    }
    printf("%ld\n", sum);

    free(arr);
    free(temp);
    return 0;
}
