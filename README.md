# barbell 🏋️

Benchmark suite for comparing language performance across Hemlock, HemlockC, C, Python, and JavaScript.

## Benchmarks

### Micro-benchmarks
- **fib** - Recursive Fibonacci (CPU-bound, function call overhead)
- **array_sum** - Sum large array (memory access, iteration)
- **string_concat** - String concatenation (allocation, GC pressure)
- **hash_ops** - Hash table insert/lookup (common real-world pattern)

### Real-world
- **json_parse** - Parse JSON file (practical workload)
- **primes_sieve** - Sieve of Eratosthenes (memory + compute)

## Usage

```bash
./run.sh              # Run all benchmarks
./run.sh fib          # Run specific benchmark
./run.sh --quick      # Quick mode (smaller inputs)
```

## Results

Results are output as JSON for easy comparison and visualization.

## Adding a benchmark

1. Create implementations in `benchmarks/<name>/` for each language
2. Add benchmark config to `benchmarks.json`
3. Run `./run.sh <name>` to test
