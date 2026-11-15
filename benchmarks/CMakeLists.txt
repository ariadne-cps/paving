# Benchmarking registers this directory in standalone builds. The production
# Paving library already bundles Threading/Logging objects through BDD.
# The framework is linked only by the dedicated benchmark executables.
add_executable(benchmark_paving_backends benchmark_paving_backends.cpp)
target_link_libraries(benchmark_paving_backends PRIVATE
    ariadne-paving paving ariadne-benchmarking)
add_executable(benchmark_paving_superset benchmark_paving_superset.cpp)
target_link_libraries(benchmark_paving_superset PRIVATE
    ariadne-paving paving ariadne-benchmarking)
add_dependencies(benchmarks benchmark_paving_backends benchmark_paving_superset)
# Manual-only comparison of Paver algorithms on shared certified geometry.
add_executable(benchmark_pavers benchmark_pavers.cpp)
target_link_libraries(benchmark_pavers PRIVATE
    ariadne-paving paving ariadne-benchmarking)
add_dependencies(benchmarks benchmark_pavers)

# Manual-only enclosure-sequence comparison resembling reachability workloads.
add_executable(benchmark_paver_sequences benchmark_paver_sequences.cpp)
target_link_libraries(benchmark_paver_sequences PRIVATE
    ariadne-paving paving ariadne-benchmarking)
add_dependencies(benchmarks benchmark_paver_sequences)

# Manual-only Tree snapshot, BDD compilation and query break-even study.
add_executable(benchmark_cached_binary_tree_paving benchmark_cached_binary_tree_paving.cpp)
target_link_libraries(benchmark_cached_binary_tree_paving PRIVATE
    ariadne-paving paving ariadne-benchmarking)
add_dependencies(benchmarks benchmark_cached_binary_tree_paving)

# Optional real SPRE22 replay. Do not substitute a synthetic fixture.
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/fixtures/spre22_enclosures.generated.hpp")
    add_executable(benchmark_spre22_fixture benchmark_spre22_fixture.cpp)
    target_link_libraries(benchmark_spre22_fixture PRIVATE
        ariadne-paving paving ariadne-benchmarking)
    # The captured 48-enclosure header expands into one extremely large
    # inline factory. GCC can skip memory-intensive optimization passes
    # for this function and report -Wdisabled-optimization. Do not turn
    # off -Werror globally: downgrade only that warning, only for this
    # optional benchmark target, and only when compiling with GCC.
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_compile_options(benchmark_spre22_fixture PRIVATE
            -Wno-error=disabled-optimization)
    endif()
    add_dependencies(benchmarks benchmark_spre22_fixture)
else()
    message(STATUS "Real SPRE22 fixture absent; generate it from ARCH before building benchmark_spre22_fixture")
endif()
