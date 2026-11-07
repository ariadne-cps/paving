# Match ariadne-cps/numeric: benchmarks are explicit executables and
# are not included in the ordinary library build.
if(NOT TARGET benchmarks)
    add_custom_target(benchmarks)
endif()

add_executable(benchmark_paving_backends benchmark_paving_backends.cpp)
target_link_libraries(benchmark_paving_backends PRIVATE ariadne-paving paving)
add_dependencies(benchmarks benchmark_paving_backends)

# Dedicated read-only cell-containment microbenchmark. Its unit timings
# exclude geometric classification and never change the manager on queries.
add_executable(benchmark_paving_superset benchmark_paving_superset.cpp)
target_link_libraries(benchmark_paving_superset PRIVATE ariadne-paving paving)
add_dependencies(benchmarks benchmark_paving_superset)

# Opt-in traversal diagnosis on prebuilt trees; intentionally outside CTest.
add_executable(benchmark_binary_tree_cursor benchmark_binary_tree_cursor.cpp)
target_link_libraries(benchmark_binary_tree_cursor PRIVATE ariadne-paving paving)
add_dependencies(benchmarks benchmark_binary_tree_cursor)
