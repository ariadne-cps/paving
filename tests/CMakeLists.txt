include(CTest)
set(UNIT_TESTS
    test_affine_set_approximation
    test_grid
    test_grid_cell_alignment
    test_binary_tree_paving
    test_paving_backends
    test_paving_arithmetic
    test_paving_concurrent_adjoin
    test_paving_lock_reentrancy
    test_robdd_paving
    test_cached_binary_tree_paving
    test_paver
    test_paving
    test_augmented_paving
    test_set_approximation
)
foreach(TEST IN LISTS UNIT_TESTS)
    add_executable(${TEST} ${TEST}.cpp)
    target_link_libraries(${TEST} PRIVATE ariadne-paving paving ariadne-testing)
    add_test(NAME ${TEST} COMMAND $<TARGET_FILE:${TEST}> --concise)
    set_property(TEST ${TEST} PROPERTY LABELS "paving")
    if(WIN32)
        set_tests_properties(${TEST} PROPERTIES ENVIRONMENT_MODIFICATION
            "PATH=path_list_prepend:$<TARGET_FILE_DIR:ariadne-paving>")
    endif()
endforeach()
add_custom_target(tests)
add_dependencies(tests ${UNIT_TESTS})

# A deadlock in a standalone owner-to-owner operation must fail promptly
# rather than consuming the entire CI job timeout.
set_tests_properties(test_paving_lock_reentrancy PROPERTIES TIMEOUT 20)
set_tests_properties(test_paving_concurrent_adjoin PROPERTIES TIMEOUT 60)
