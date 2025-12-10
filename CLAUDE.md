# Barbell - Hemlock Benchmark Suite

## Purpose
Barbell is a benchmark suite for comparing Hemlock's performance against C, Python, and JavaScript. Includes both interpreted Hemlock and compiled Hemlock (hemlockc) benchmarks.

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

# Use local hemlockc (compiled hemlock) build
HEMLOCKC_BIN="../hemlock/hemlockc" ./run.sh
```

## Benchmarks

- **fib** - Recursive Fibonacci (function call overhead, recursion)
- **array_sum** - Sum large array (memory, iteration)
- **string_concat** - String concatenation (allocation, GC pressure)
- **primes_sieve** - Sieve of Eratosthenes (array access, loops)
- **json_serialize** - JSON serialization (object creation, string building)
- **json_deserialize** - JSON parsing (string parsing, object creation)
- **word_count** - Count words/lines in large text (classic MapReduce task)

## Notes

### Hemlock Performance Observations

**Interpreted Hemlock** is significantly slower than Python/JS on compute-heavy tasks:
- fib: ~30x slower than C
- array operations: ~138x slower than C (bytecode interpretation overhead)
- string_concat: Competitive with other languages

**Compiled Hemlock (hemlockc)** shows substantial improvement over interpreted:
- fib: ~3-5x slower than C (9x faster than interpreted)
- array_sum: ~3x slower than C (40x faster than interpreted)
- string_concat: Near C performance (~1.1x)
- primes_sieve: ~1.5x slower than C (12x faster than interpreted)

This demonstrates that hemlockc provides significant performance gains for compute-heavy tasks.

### Hemlock Quirks Discovered

1. **Rune to int conversion**: Must use type annotation (e.g., `let code: i32 = ch;`) to convert runes to integers for arithmetic

2. **Variable shadowing fixed in v1.0.6**: Prior to v1.0.6, function parameters with the same name as outer variables would incorrectly modify the outer variable instead of creating a local binding. This was fixed by changing `env_set` to `env_define` for parameter binding.

### libwebsockets Logging

Hemlock uses libwebsockets for HTTP/WebSocket support. To suppress the verbose startup messages:
- Default: Only errors shown
- Verbose: Set `LWS_VERBOSE=1` environment variable

This was implemented in `websockets.c` using `lws_set_log_level()`.
