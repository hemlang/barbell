def merge(arr, left, mid, right, temp)
  i, j, k = left, mid + 1, left

  while i <= mid && j <= right
    if arr[i] <= arr[j]
      temp[k] = arr[i]
      i += 1
    else
      temp[k] = arr[j]
      j += 1
    end
    k += 1
  end

  while i <= mid
    temp[k] = arr[i]
    i += 1
    k += 1
  end

  while j <= right
    temp[k] = arr[j]
    j += 1
    k += 1
  end

  (left..right).each { |x| arr[x] = temp[x] }
end

def merge_sort(arr, left, right, temp)
  if left < right
    mid = left + (right - left) / 2
    merge_sort(arr, left, mid, temp)
    merge_sort(arr, mid + 1, right, temp)
    merge(arr, left, mid, right, temp)
  end
end

n = ARGV[0] ? ARGV[0].to_i : 100000

# Fill with descending values
arr = (1..n).to_a.reverse
temp = Array.new(n, 0)

merge_sort(arr, 0, n - 1, temp)

# Print checksum
puts arr.sum
