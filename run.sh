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
    echo "Benchmarks: fib, array_sum, string_concat, primes_sieve, http_throughput"
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
        http_throughput)
            # Number of requests (keep low due to connection overhead)
            [[ $QUICK_MODE -eq 1 ]] && echo 500 || echo 2000
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

# Run HTTP throughput benchmark (special handling - measures requests/second)
run_http_benchmark() {
    local lang=$1
    local num_requests=$2
    local bench_dir="$SCRIPT_DIR/benchmarks/http_throughput"
    local client="$bench_dir/http_client.py"
    local port=8765
    local sum=0
    local count=0
    local server_pid

    # Find an available port
    while netstat -tuln 2>/dev/null | grep -q ":$port " || ss -tuln 2>/dev/null | grep -q ":$port "; do
        port=$((port + 1))
    done

    for ((i=0; i<ITERATIONS; i++)); do
        # Start server based on language
        case $lang in
            c)
                local src="$bench_dir/http_throughput.c"
                local bin="$BUILD_DIR/http_throughput_c"
                [[ ! -f "$src" ]] && return 1
                gcc -O3 -o "$bin" "$src" 2>/dev/null || return 1
                "$bin" "$port" &
                server_pid=$!
                ;;
            hemlock)
                local src="$bench_dir/http_throughput.hml"
                [[ ! -f "$src" ]] && return 1
                local hemlock_bin="${HEMLOCK_BIN:-hemlock}"
                "$hemlock_bin" "$src" "$port" &
                server_pid=$!
                ;;
            hemlockc)
                local src="$bench_dir/http_throughput.hml"
                local bin="$BUILD_DIR/http_throughput_hemlockc"
                [[ ! -f "$src" ]] && return 1
                local hemlockc_bin="${HEMLOCKC_BIN:-hemlockc}"
                local runtime_dir="${HEMLOCK_RUNTIME:-}"
                if [[ -z "$runtime_dir" ]]; then
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
                "$hemlockc_bin" -O3 "$src" -o "$bin" 2>/dev/null || return 1
                "$bin" "$port" &
                server_pid=$!
                ;;
            python)
                local src="$bench_dir/http_throughput.py"
                [[ ! -f "$src" ]] && return 1
                python3 "$src" "$port" &
                server_pid=$!
                ;;
            javascript)
                local src="$bench_dir/http_throughput.js"
                [[ ! -f "$src" ]] && return 1
                node "$src" "$port" &
                server_pid=$!
                ;;
            ruby)
                local src="$bench_dir/http_throughput.rb"
                [[ ! -f "$src" ]] && return 1
                ruby "$src" "$port" &
                server_pid=$!
                ;;
            *)
                return 1
                ;;
        esac

        # Wait for server to be ready (max 5 seconds)
        local ready=0
        for ((j=0; j<50; j++)); do
            if curl -s --max-time 1 "http://127.0.0.1:$port/" >/dev/null 2>&1; then
                ready=1
                break
            fi
            sleep 0.1
        done

        if [[ $ready -eq 0 ]]; then
            kill "$server_pid" 2>/dev/null
            wait "$server_pid" 2>/dev/null
            return 1
        fi

        # Run client and get requests/second
        local rps
        rps=$(python3 "$client" --port "$port" --requests "$num_requests" 2>/dev/null)

        # Stop server
        kill "$server_pid" 2>/dev/null
        wait "$server_pid" 2>/dev/null

        if [[ -n "$rps" ]]; then
            # Convert to integer for averaging (multiply by 100 for precision)
            local rps_int=$(echo "$rps * 100" | bc | cut -d. -f1)
            sum=$((sum + rps_int))
            count=$((count + 1))
        fi

        # Small delay between iterations
        sleep 0.2
    done

    [[ $count -eq 0 ]] && return 1
    # Return average (divide by 100 to get back to original scale)
    echo "scale=2; $sum / $count / 100" | bc
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
        benchmarks="fib array_sum string_concat primes_sieve http_throughput"
    fi

    local languages="c hemlockc hemlock python javascript ruby"

    echo -e "${BOLD}barbell${NC} - Hemlock Benchmark Suite"
    echo ""
    [[ $QUICK_MODE -eq 1 ]] && echo -e "${YELLOW}Quick mode enabled${NC}"
    echo -e "Iterations: $ITERATIONS"
    echo ""

    for bench in $benchmarks; do
        local input=$(get_input "$bench")

        if [[ "$bench" == "http_throughput" ]]; then
            echo -e "${BOLD}${BLUE}$bench${NC} (requests=$input)"
            echo "─────────────────────────────────"

            local c_rps=0

            for lang in $languages; do
                local rps
                rps=$(run_http_benchmark "$lang" "$input" 2>/dev/null) || continue

                [[ $lang == "c" ]] && c_rps=$(echo "$rps" | bc)

                local formatted=$(printf "%.0f req/s" "$rps")
                local ratio=""
                if [[ $(echo "$c_rps > 0" | bc) -eq 1 && $lang != "c" ]]; then
                    # For throughput, higher is better, so ratio < 1 means slower
                    ratio=$(printf " (%.2fx)" "$(echo "scale=2; $rps / $c_rps" | bc)")
                fi

                printf "  %-12s %12s%s\n" "$lang" "$formatted" "$ratio"
            done
        else
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
        fi
        echo ""
    done
}

run_all
