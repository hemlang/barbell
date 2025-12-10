import sys
sys.setrecursionlimit(20000)

def partition(arr, low, high):
    pivot = arr[high]
    i = low - 1
    for j in range(low, high):
        if arr[j] <= pivot:
            i += 1
            arr[i], arr[j] = arr[j], arr[i]
    arr[i + 1], arr[high] = arr[high], arr[i + 1]
    return i + 1

def quicksort(arr, low, high):
    if low < high:
        pi = partition(arr, low, high)
        quicksort(arr, low, pi - 1)
        quicksort(arr, pi + 1, high)

n = int(sys.argv[1]) if len(sys.argv) > 1 else 100000

# Fill with descending values
arr = list(range(n, 0, -1))

quicksort(arr, 0, n - 1)

# Print checksum
print(sum(arr))
