# Match ariadne-cps/numeric: benchmarks are explicit executables and
# are not included in the ordinary library build.
if(NOT TARGET benchmarks)
    add_custom_target(benchmarks)
endif()

add_executable(benchmark_paving_backends benchmark_paving_backends.cpp)
target_link_libraries(benchmark_paving_backends PRIVATE ariadne-paving paving)
add_dependencies(benchmarks benchmark_paving_backends)

# Ensure the benchmark compiles on supported CI platforms. This smoke
# case is deliberately small and checks the executable, not performance.
add_dependencies(tests benchmark_paving_backends)
add_test(
    NAME benchmark_paving_backends_smoke
    COMMAND $<TARGET_FILE:benchmark_paving_backends>
        --dimensions 2 --finenesses 0 --shapes aligned-box
        --iterations 1 --samples 3 --max-depth-bits 12
        --min-leaves 0 --min-sample-ms 1 --max-iterations 2
        --concurrency 0 --retention-steps 2 --retention-fineness 2
        --summary-only
)
set_property(TEST benchmark_paving_backends_smoke PROPERTY LABELS "paving;benchmark-smoke")
if(WIN32)
    set_tests_properties(benchmark_paving_backends_smoke PROPERTIES
        ENVIRONMENT_MODIFICATION
            "PATH=path_list_prepend:$<TARGET_FILE_DIR:ariadne-paving>")
endif()
