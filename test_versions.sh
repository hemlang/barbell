#!/bin/bash
# Test hemlock performance across different versions

HEMLOCK_DIR="/home/user/hemlock"
BARBELL_DIR="/home/user/barbell"

# Selected versions to test
VERSIONS="v1.0.6 v1.1.0 v1.1.3 v1.3.0"

# Key benchmarks to test
BENCHMARKS="fib array_sum string_concat primes_sieve"

# Store results
declare -A HEMLOCK_RESULTS
declare -A HEMLOCKC_RESULTS

build_version() {
    local version=$1
    cd "$HEMLOCK_DIR"

    # Clean
    make clean 2>/dev/null || true
    rm -rf build hemlock hemlockc 2>/dev/null || true

    # Checkout
    git checkout "$version" 2>&1 | grep -v "^Already" | grep -v "^Switched" || true

    # Build
    if make -j4 2>&1 >/dev/null; then
        return 0
    else
        return 1
    fi
}

run_single_benchmark() {
    local bench=$1
    local lang=$2
    local input=$3
    local hemlock_bin=$4
    local hemlockc_bin=$5

    cd "$BARBELL_DIR"
    local src="benchmarks/$bench/${bench}.hml"

    if [[ ! -f "$src" ]]; then
        echo "N/A"
        return
    fi

    local start end time_ms

    if [[ "$lang" == "hemlock" && -n "$hemlock_bin" && -f "$hemlock_bin" ]]; then
        start=$(perl -MTime::HiRes=time -e 'printf "%d\n", time * 1000')
        if timeout 30 "$hemlock_bin" "$src" "$input" > /dev/null 2>&1; then
            end=$(perl -MTime::HiRes=time -e 'printf "%d\n", time * 1000')
            time_ms=$((end - start))
            echo "$time_ms"
        else
            echo "ERR"
        fi
    elif [[ "$lang" == "hemlockc" && -n "$hemlockc_bin" && -f "$hemlockc_bin" ]]; then
        local bin="$BARBELL_DIR/build/${bench}_hemlockc_test"
        mkdir -p "$BARBELL_DIR/build"

        # Set up paths for runtime
        export C_INCLUDE_PATH="$HEMLOCK_DIR/runtime/include:${C_INCLUDE_PATH:-}"
        export LIBRARY_PATH="$HEMLOCK_DIR:${LIBRARY_PATH:-}"
        export LD_LIBRARY_PATH="$HEMLOCK_DIR:${LD_LIBRARY_PATH:-}"

        # Compile
        if ! "$hemlockc_bin" -O3 "$src" -o "$bin" 2>/dev/null; then
            echo "N/A"
            return
        fi

        start=$(perl -MTime::HiRes=time -e 'printf "%d\n", time * 1000')
        if timeout 30 "$bin" "$input" > /dev/null 2>&1; then
            end=$(perl -MTime::HiRes=time -e 'printf "%d\n", time * 1000')
            time_ms=$((end - start))
            echo "$time_ms"
        else
            echo "ERR"
        fi
        rm -f "$bin"
    else
        echo "N/A"
    fi
}

# Quick mode inputs
get_input() {
    case $1 in
        fib) echo 20 ;;
        array_sum) echo 100000 ;;
        string_concat) echo 5000 ;;
        primes_sieve) echo 10000 ;;
        *) echo 1000 ;;
    esac
}

echo "Building and testing Hemlock versions..."
echo ""

# Test each version
for VERSION in $VERSIONS; do
    echo -n "Testing $VERSION... "

    if ! build_version "$VERSION"; then
        echo "BUILD FAILED"
        for bench in $BENCHMARKS; do
            HEMLOCK_RESULTS["$VERSION,$bench"]="BUILD_FAIL"
            HEMLOCKC_RESULTS["$VERSION,$bench"]="BUILD_FAIL"
        done
        continue
    fi

    HEMLOCK_BIN="$HEMLOCK_DIR/hemlock"
    HEMLOCKC_BIN="$HEMLOCK_DIR/hemlockc"

    for bench in $BENCHMARKS; do
        input=$(get_input "$bench")

        # Test hemlock
        result=$(run_single_benchmark "$bench" "hemlock" "$input" "$HEMLOCK_BIN" "")
        HEMLOCK_RESULTS["$VERSION,$bench"]="$result"

        # Test hemlockc
        result=$(run_single_benchmark "$bench" "hemlockc" "$input" "" "$HEMLOCKC_BIN")
        HEMLOCKC_RESULTS["$VERSION,$bench"]="$result"
    done

    echo "done"
done

# Return to main branch
cd "$HEMLOCK_DIR"
git checkout main 2>/dev/null || git checkout master 2>/dev/null || true

# Print results table
echo ""
echo "================================================================================"
echo "                    HEMLOCK INTERPRETER PERFORMANCE (ms)"
echo "================================================================================"
printf "%-12s" "Version"
for bench in $BENCHMARKS; do
    printf "%15s" "$bench"
done
echo ""
echo "--------------------------------------------------------------------------------"

for VERSION in $VERSIONS; do
    printf "%-12s" "$VERSION"
    for bench in $BENCHMARKS; do
        result="${HEMLOCK_RESULTS[$VERSION,$bench]}"
        if [[ "$result" == "N/A" || "$result" == "ERR" || "$result" == "BUILD_FAIL" ]]; then
            printf "%15s" "$result"
        else
            printf "%13sms" "$result"
        fi
    done
    echo ""
done

echo ""
echo "================================================================================"
echo "                    HEMLOCKC COMPILED PERFORMANCE (ms)"
echo "================================================================================"
printf "%-12s" "Version"
for bench in $BENCHMARKS; do
    printf "%15s" "$bench"
done
echo ""
echo "--------------------------------------------------------------------------------"

for VERSION in $VERSIONS; do
    printf "%-12s" "$VERSION"
    for bench in $BENCHMARKS; do
        result="${HEMLOCKC_RESULTS[$VERSION,$bench]}"
        if [[ "$result" == "N/A" || "$result" == "ERR" || "$result" == "BUILD_FAIL" ]]; then
            printf "%15s" "$result"
        else
            printf "%13sms" "$result"
        fi
    done
    echo ""
done

echo ""
