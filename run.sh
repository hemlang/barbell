#!/usr/bin/env bash
# barbell - Benchmark runner for Hemlock language comparisons

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$SCRIPT_DIR/build"
RESULTS_DIR="$SCRIPT_DIR/results"

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
BOLD='\033[1m'
NC='\033[0m'

# Defaults
QUICK_MODE=0
BENCHMARK=""
ITERATIONS=3

usage() {
    echo "Usage: $0 [OPTIONS] [BENCHMARK]"
    echo ""
    echo "Options:"
    echo "  --quick, -q     Use smaller inputs for faster runs"
    echo "  --iter N        Number of iterations (default: 3)"
    echo "  --help, -h      Show this help"
    echo ""
    echo "Benchmarks: fib, array_sum, string_concat, primes_sieve, json_serialize, json_deserialize, unicode_normalize"
    echo "            (leave empty to run all)"
}

# Parse arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        --quick|-q)
            QUICK_MODE=1
            shift
            ;;
        --iter)
            ITERATIONS="$2"
            shift 2
            ;;
        --help|-h)
            usage
            exit 0
            ;;
        *)
            BENCHMARK="$1"
            shift
            ;;
    esac
done

mkdir -p "$BUILD_DIR" "$RESULTS_DIR"

# Get benchmark input size
get_input() {
    local bench=$1
    case $bench in
        fib)
            [[ $QUICK_MODE -eq 1 ]] && echo 30 || echo 35
            ;;
        array_sum)
            [[ $QUICK_MODE -eq 1 ]] && echo 1000000 || echo 10000000
            ;;
        string_concat)
            [[ $QUICK_MODE -eq 1 ]] && echo 10000 || echo 100000
            ;;
        primes_sieve)
            [[ $QUICK_MODE -eq 1 ]] && echo 100000 || echo 1000000
            ;;
        json_serialize)
            [[ $QUICK_MODE -eq 1 ]] && echo 10000 || echo 100000
            ;;
        json_deserialize)
            [[ $QUICK_MODE -eq 1 ]] && echo 10000 || echo 100000
            ;;
        unicode_normalize)
            [[ $QUICK_MODE -eq 1 ]] && echo 10000 || echo 100000
            ;;
    esac
}

# Time a command and return milliseconds
time_cmd() {
    local start end
    start=$(python3 -c 'import time; print(int(time.time() * 1000))')
    eval "$@" > /dev/null 2>&1
    end=$(python3 -c 'import time; print(int(time.time() * 1000))')
    echo $((end - start))
}

# Run a single benchmark for a language
run_benchmark() {
    local bench=$1
    local lang=$2
    local input=$3
    local bench_dir="$SCRIPT_DIR/benchmarks/$bench"
    local sum=0
    local count=0

    case $lang in
        c)
            local src="$bench_dir/${bench}.c"
            local bin="$BUILD_DIR/${bench}_c"
            [[ ! -f "$src" ]] && return 1
            gcc -O3 -o "$bin" "$src" 2>/dev/null || return 1
            for ((i=0; i<ITERATIONS; i++)); do
                t=$(time_cmd "$bin" "$input")
                sum=$((sum + t))
                count=$((count + 1))
            done
            ;;
        hemlock)
            local src="$bench_dir/${bench}.hml"
            [[ ! -f "$src" ]] && return 1
            # Use local hemlock if available, otherwise system hemlock
            local hemlock_bin="${HEMLOCK_BIN:-hemlock}"
            for ((i=0; i<ITERATIONS; i++)); do
                t=$(time_cmd "$hemlock_bin" "$src" "$input")
                sum=$((sum + t))
                count=$((count + 1))
            done
            ;;
        hemlockc)
            local src="$bench_dir/${bench}.hml"
            local bin="$BUILD_DIR/${bench}_hemlockc"
            [[ ! -f "$src" ]] && return 1
            # Use local hemlockc if available, otherwise system hemlockc
            local hemlockc_bin="${HEMLOCKC_BIN:-hemlockc}"
            # Runtime paths - check for local hemlock build or use HEMLOCK_RUNTIME env var
            local runtime_dir="${HEMLOCK_RUNTIME:-}"
            if [[ -z "$runtime_dir" ]]; then
                # Try common locations
                for dir in "$SCRIPT_DIR/../hemlock" "$HOME/Projects/hemlock"; do
                    if [[ -f "$dir/libhemlock_runtime.so" ]]; then
                        runtime_dir="$dir"
                        break
                    fi
                done
            fi
            if [[ -n "$runtime_dir" ]]; then
                export C_INCLUDE_PATH="${runtime_dir}/runtime/include:${C_INCLUDE_PATH:-}"
                export LIBRARY_PATH="${runtime_dir}:${LIBRARY_PATH:-}"
                export LD_LIBRARY_PATH="${runtime_dir}:${LD_LIBRARY_PATH:-}"
            fi
            # Compile with optimization
            "$hemlockc_bin" -O3 "$src" -o "$bin" 2>/dev/null || return 1
            for ((i=0; i<ITERATIONS; i++)); do
                t=$(time_cmd "$bin" "$input")
                sum=$((sum + t))
                count=$((count + 1))
            done
            ;;
        python)
            local src="$bench_dir/${bench}.py"
            [[ ! -f "$src" ]] && return 1
            for ((i=0; i<ITERATIONS; i++)); do
                t=$(time_cmd python3 "$src" "$input")
                sum=$((sum + t))
                count=$((count + 1))
            done
            ;;
        javascript)
            local src="$bench_dir/${bench}.js"
            [[ ! -f "$src" ]] && return 1
            for ((i=0; i<ITERATIONS; i++)); do
                t=$(time_cmd node "$src" "$input")
                sum=$((sum + t))
                count=$((count + 1))
            done
            ;;
        ruby)
            local src="$bench_dir/${bench}.rb"
            [[ ! -f "$src" ]] && return 1
            for ((i=0; i<ITERATIONS; i++)); do
                t=$(time_cmd ruby "$src" "$input")
                sum=$((sum + t))
                count=$((count + 1))
            done
            ;;
    esac

    [[ $count -eq 0 ]] && return 1
    echo $((sum / count))
}

# Format time nicely
format_time() {
    local ms=$1
    if [[ $ms -ge 1000 ]]; then
        printf "%.2fs" "$(echo "scale=2; $ms / 1000" | bc)"
    else
        printf "%dms" "$ms"
    fi
}

# Run benchmarks
run_all() {
    local benchmarks
    if [[ -n "$BENCHMARK" ]]; then
        benchmarks="$BENCHMARK"
    else
        benchmarks="fib array_sum string_concat primes_sieve json_serialize json_deserialize unicode_normalize"
    fi

    local languages="c hemlockc hemlock python javascript ruby"

    echo -e "${BOLD}barbell${NC} - Hemlock Benchmark Suite"
    echo ""
    [[ $QUICK_MODE -eq 1 ]] && echo -e "${YELLOW}Quick mode enabled${NC}"
    echo -e "Iterations: $ITERATIONS"
    echo ""

    for bench in $benchmarks; do
        local input=$(get_input "$bench")
        echo -e "${BOLD}${BLUE}$bench${NC} (n=$input)"
        echo "─────────────────────────────────"

        local c_time=0

        for lang in $languages; do
            local time_ms
            time_ms=$(run_benchmark "$bench" "$lang" "$input" 2>/dev/null) || continue

            [[ $lang == "c" ]] && c_time=$time_ms

            local formatted=$(format_time "$time_ms")
            local ratio=""
            if [[ $c_time -gt 0 && $lang != "c" ]]; then
                ratio=$(printf " (%.1fx)" "$(echo "scale=1; $time_ms / $c_time" | bc)")
            fi

            printf "  %-12s %8s%s\n" "$lang" "$formatted" "$ratio"
        done
        echo ""
    done
}

run_all
