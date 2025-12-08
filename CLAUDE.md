# Barbell - Hemlock Benchmark Suite

## Purpose
Barbell is a benchmark suite for comparing Hemlock's performance against C, Python, and JavaScript.

## Running Benchmarks

```bash
# Run all benchmarks
./run.sh

# Quick mode (smaller inputs)
./run.sh --quick

# Single benchmark
./run.sh fib

# Use local hemlock build
HEMLOCK_BIN="../hemlock/hemlock" ./run.sh
```

## Benchmarks

- **fib** - Recursive Fibonacci (function call overhead, recursion)
- **array_sum** - Sum large array (memory, iteration)
- **string_concat** - String concatenation (allocation, GC pressure)
- **primes_sieve** - Sieve of Eratosthenes (array access, loops)

## Notes

### Hemlock Performance Observations

Initial benchmarks show Hemlock is significantly slower than Python/JS on compute-heavy tasks:
- fib: ~46x slower than C, ~20x slower than Python
- array operations: Very slow due to interpreted bytecode
- string_concat: Competitive with other languages

This provides a baseline for future optimization work.

### Hemlock Quirks Discovered

1. **Rune to int conversion**: Must use type annotation (e.g., `let code: i32 = ch;`) to convert runes to integers for arithmetic

2. **Variable shadowing fixed in v1.0.6**: Prior to v1.0.6, function parameters with the same name as outer variables would incorrectly modify the outer variable instead of creating a local binding. This was fixed by changing `env_set` to `env_define` for parameter binding.

### libwebsockets Logging

Hemlock uses libwebsockets for HTTP/WebSocket support. To suppress the verbose startup messages:
- Default: Only errors shown
- Verbose: Set `LWS_VERBOSE=1` environment variable

This was implemented in `websockets.c` using `lws_set_log_level()`.
