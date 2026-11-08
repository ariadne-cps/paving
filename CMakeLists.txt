cmake_minimum_required(VERSION 3.15)

project(Paving VERSION 1.0)

option(WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)
option(BUILD_PYTHON_BINDINGS "Build Python bindings" ON)

if(NOT TARGET ariadne-build)
    find_package(Git REQUIRED)
    # Keep initial Build checkout output in English.
    execute_process(
        COMMAND "${CMAKE_COMMAND}" -E env "LC_ALL=C"
                "${GIT_EXECUTABLE}" -C "${CMAKE_CURRENT_SOURCE_DIR}"
                submodule update --init -- ss/bui
        RESULT_VARIABLE _result)
    if(_result)
        message(FATAL_ERROR "Cannot initialize build submodule")
    endif()
    include("${CMAKE_CURRENT_SOURCE_DIR}/ss/bui/cmake/ProjectOptions.cmake")
endif()

if(EXISTS "${PROJECT_SOURCE_DIR}/.git")
    # Child commits guarantee their own internal consistency.
    # Keep cross-branch comparisons where the same dependency is shared.
    require_same_dependency_commit(Build CHAINS
        "build" "set|build" "bdd|build" "testing|build")
    require_same_dependency_commit(Foundation CHAINS
        "tree|foundation"
        "set|linear-programming|algebra|interval|numeric|foundation")
    require_same_dependency_commit(Logging CHAINS
        "set|linear-programming|algebra|interval|numeric|logging"
        "bdd|threading|logging")
    require_same_dependency_commit(PythonCommon CHAINS
        "set|linear-programming|algebra|interval|numeric|foundation|python-common"
        "bdd|python-common")
endif()

# This source-file change bootstraps the first reconfigure on upgrades
# from older Build gitlinks that did not yet watch the Git index.
# Subsequent pulls are detected by Build's centralized setup_project().
setup_project()

include_directories(${PROJECT_SOURCE_DIR}/include)
add_subdirectory(ss)

add_library(paving INTERFACE)
target_include_directories(paving INTERFACE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include/ariadne-paving>
)
target_link_libraries(paving INTERFACE set bdd tree)

add_subdirectory(src)
target_link_libraries(PAVING_SRC PRIVATE paving)

ariadne_register_public_headers(
    TARGET paving
    DIRECTORIES "${PROJECT_SOURCE_DIR}/include/paving"
    FILES "${PROJECT_SOURCE_DIR}/include/ariadne-paving.hpp"
)

setup_project_library(
    TARGET ariadne-paving
    OBJECTS PAVING_SRC
    LINK_LIBRARIES paving
)

setup_standalone_project_tests(
    TARGET ariadne-paving
    TEST_DIRECTORY tests
    TEST_TARGET tests
    SOURCES
        "${PROJECT_SOURCE_DIR}/include"
        "${PROJECT_SOURCE_DIR}/src"
)

if(BUILD_PYTHON_BINDINGS)
    add_subdirectory(python)
endif()

if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    ariadne_install_dependency_bundle(
        TARGET paving
        DESTINATION include/ariadne-paving
    )
endif()
