include(CTest)
set(UNIT_TESTS
    test_affine_set_approximation
    test_binary_tree
    test_grid
    test_grid_paving
    test_grid_paving_extensions
    test_paver
    test_paving
    test_set_approximation
)
foreach(TEST IN LISTS UNIT_TESTS)
    add_executable(${TEST} ${TEST}.cpp)
    target_link_libraries(${TEST} PRIVATE ariadne-paving paving)
    add_test(NAME ${TEST} COMMAND $<TARGET_FILE:${TEST}>)
    set_property(TEST ${TEST} PROPERTY LABELS "paving")
    if(WIN32)
        set_tests_properties(${TEST} PROPERTIES ENVIRONMENT_MODIFICATION
            "PATH=path_list_prepend:$<TARGET_FILE_DIR:ariadne-paving>")
    endif()
endforeach()
add_custom_target(tests)
add_dependencies(tests ${UNIT_TESTS})
