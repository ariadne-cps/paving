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

# Manual-only Tree snapshot, BDD compilation and query break-even study.
add_executable(benchmark_cached_binary_tree_paving benchmark_cached_binary_tree_paving.cpp)
target_link_libraries(benchmark_cached_binary_tree_paving PRIVATE
    ariadne-paving paving ariadne-benchmarking)
add_dependencies(benchmarks benchmark_cached_binary_tree_paving)

# Manual-only serial vs parallel certified outer classification.
add_executable(benchmark_parallel_outer_paving benchmark_parallel_outer_paving.cpp)
target_link_libraries(benchmark_parallel_outer_paving PRIVATE
    ariadne-paving paving ariadne-benchmarking)
add_dependencies(benchmarks benchmark_parallel_outer_paving)
