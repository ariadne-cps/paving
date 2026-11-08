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
