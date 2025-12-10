def partition(arr, low, high)
  pivot = arr[high]
  i = low - 1
  (low...high).each do |j|
    if arr[j] <= pivot
      i += 1
      arr[i], arr[j] = arr[j], arr[i]
    end
  end
  arr[i + 1], arr[high] = arr[high], arr[i + 1]
  i + 1
end

def quicksort(arr, low, high)
  if low < high
    pi = partition(arr, low, high)
    quicksort(arr, low, pi - 1)
    quicksort(arr, pi + 1, high)
  end
end

n = ARGV[0] ? ARGV[0].to_i : 100000

# Fill with descending values
arr = (1..n).to_a.reverse

quicksort(arr, 0, n - 1)

# Print checksum
puts arr.sum
