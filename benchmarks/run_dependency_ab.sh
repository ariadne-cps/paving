#!/usr/bin/env bash
# Manual four-way BDD/Tree dependency experiment. Never invoked by CI.
set -euo pipefail

root="$(cd "$(dirname "$0")/.." && pwd)"
source_sha="$(git -C "$root" rev-parse HEAD)"
output="${RESULT_DIR:-$(dirname "$root")/paving-deps-ab-$(date -u +%Y%m%dT%H%M%SZ)}"
if [[ -e "$output" ]]; then
    echo "Refusing to overwrite existing directory: $output" >&2
    exit 1
fi
mkdir -p "$output"

old_bdd=3b6c9183e0c8cebb807ee66ae1891f74c8c45c46
new_bdd=378d16e12e32641dfafacad9afab28963191d646
old_tree=7166244bc2cd22fec9e2c7aec1db38903c08b206
new_tree=ba055ee7cf5663c916a54635565fb97513b690bb

dimensions="${DIMENSIONS:-4}"
finenesses="${FINENESSES:-3}"
shapes="${SHAPES:-aligned-box,rotated-box,aligned-ellipsoid,rotated-ellipsoid,ball}"
samples="${SAMPLES:-7}"
min_ms="${MIN_SAMPLE_MS:-12}"
max_iter="${MAX_ITERATIONS:-8192}"
max_bits="${MAX_DEPTH_BITS:-28}"
workers="${CONCURRENCY:-0}"

{
    printf 'paving_sha=%s\n' "$source_sha"
    printf 'old_bdd=%s\nnew_bdd=%s\nold_tree=%s\nnew_tree=%s\n' "$old_bdd" "$new_bdd" "$old_tree" "$new_tree"
    printf 'dimensions=%s\nfinenesses=%s\nshapes=%s\n' "$dimensions" "$finenesses" "$shapes"
    printf 'samples=%s\nmin_ms=%s\nmax_iter=%s\nmax_bits=%s\nworkers=%s\n' "$samples" "$min_ms" "$max_iter" "$max_bits" "$workers"
    printf 'cmake=%s\n' "$(cmake --version | head -n 1)"
    printf 'host=%s\n' "$(uname -a)"
    printf 'original_status_begin\n'
    git -C "$root" status --porcelain
    printf 'original_status_end\n'
} > "$output/manifest.txt"

# Every variant starts from identical Paving sources, using detached
# worktrees and distinct builds. Only its git index submodule pins differ.
for variant in old-old bdd-only tree-only new-new; do
    worktree="$output/$variant"
    echo "Preparing $variant"
    git -C "$root" worktree add --detach "$worktree" "$source_sha"
    case "$variant" in
        old-old) bdd="$old_bdd"; tree="$old_tree" ;;
        bdd-only) bdd="$new_bdd"; tree="$old_tree" ;;
        tree-only) bdd="$old_bdd"; tree="$new_tree" ;;
        new-new) bdd="$new_bdd"; tree="$new_tree" ;;
    esac
    git -C "$worktree" update-index --cacheinfo 160000 "$bdd" ss/bdd
    git -C "$worktree" update-index --cacheinfo 160000 "$tree" ss/tre
    git -C "$worktree" submodule update --init --recursive

    actual_bdd="$(git -C "$worktree/ss/bdd" rev-parse HEAD)"
    actual_tree="$(git -C "$worktree/ss/tre" rev-parse HEAD)"
    if [[ "$actual_bdd" != "$bdd" || "$actual_tree" != "$tree" ]]; then
        echo "Incorrect BDD/Tree checkout in $variant" >&2
        exit 1
    fi
    printf '%s paving=%s bdd=%s tree=%s\n' "$variant" "$source_sha" "$actual_bdd" "$actual_tree" >> "$output/manifest.txt"
    build="$worktree/build-release"
    cmake -S "$worktree" -B "$build" -DCMAKE_BUILD_TYPE=Release -DBUILD_PYTHON_BINDINGS=OFF
    cmake --build "$build" --config Release --target tests benchmark_paving_backends
    (cd "$build" && ctest -C Release --output-on-failure) > "$output/$variant-ctest.log" 2>&1
    echo "Build and tests succeeded for $variant"
done

locate_benchmark() {
    local build="$1"
    if [[ -x "$build/benchmarks/benchmark_paving_backends" ]]; then
        printf '%s\n' "$build/benchmarks/benchmark_paving_backends"
    elif [[ -x "$build/benchmarks/Release/benchmark_paving_backends" ]]; then
        printf '%s\n' "$build/benchmarks/Release/benchmark_paving_backends"
    else
        echo "Missing Paving benchmark: $build" >&2
        return 1
    fi
}

# Forward/reverse sequence balances environment and run order.
round=0
for variant in old-old bdd-only tree-only new-new new-new tree-only bdd-only old-old; do
    round=$((round+1))
    log="$output/$(printf '%02d' "$round")-$variant-backends.log"
    echo "Benchmark pass $round/8: $variant"
    "$(locate_benchmark "$output/$variant/build-release")" \
        --dimensions "$dimensions" --finenesses "$finenesses" \
        --shapes "$shapes" --samples "$samples" \
        --min-sample-ms "$min_ms" --max-iterations "$max_iter" \
        --max-depth-bits "$max_bits" --no-retention \
        --tree-iterator-cost --concurrency "$workers" > "$log" 2>&1
done

echo "Completed factorial Paving A/B. Results: $output"
echo "Compare absolute timings for the SAME backend, operation and geometry."
