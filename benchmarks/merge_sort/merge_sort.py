import sys
import time
sys.setrecursionlimit(50000)

def merge(arr, left, mid, right, temp):
    i, j, k = left, mid + 1, left

    while i <= mid and j <= right:
        if arr[i] <= arr[j]:
            temp[k] = arr[i]
            i += 1
        else:
            temp[k] = arr[j]
            j += 1
        k += 1

    while i <= mid:
        temp[k] = arr[i]
        i += 1
        k += 1

    while j <= right:
        temp[k] = arr[j]
        j += 1
        k += 1

    for i in range(left, right + 1):
        arr[i] = temp[i]

def merge_sort(arr, left, right, temp):
    if left < right:
        mid = left + (right - left) // 2
        merge_sort(arr, left, mid, temp)
        merge_sort(arr, mid + 1, right, temp)
        merge(arr, left, mid, right, temp)

n = int(sys.argv[1]) if len(sys.argv) > 1 else 100000

start = time.perf_counter()

# Fill with descending values
arr = list(range(n, 0, -1))
temp = [0] * n

merge_sort(arr, 0, n - 1, temp)

# Print checksum
result = sum(arr)

elapsed = (time.perf_counter() - start) * 1000
print(f"TIME_MS:{elapsed:.2f}", file=sys.stderr)
print(result)
