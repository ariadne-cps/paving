cmake_minimum_required(VERSION 3.15)

project(Paving VERSION 1.0)

option(WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)
option(BUILD_PYTHON_BINDINGS "Build Python bindings" ON)

if(NOT TARGET ariadne-configuration)
    find_package(Git REQUIRED)
    execute_process(COMMAND "${GIT_EXECUTABLE}" -C "${CMAKE_CURRENT_SOURCE_DIR}"
                    submodule update --init -- ss/cfg
                    RESULT_VARIABLE _result)
    if(_result)
        message(FATAL_ERROR "Cannot initialize configuration submodule")
    endif()
    include("${CMAKE_CURRENT_SOURCE_DIR}/ss/cfg/cmake/ProjectOptions.cmake")
endif()

if(EXISTS "${PROJECT_SOURCE_DIR}/.git")
    require_same_dependency_commit(Configuration
        CHAINS
            "configuration"
            "set|configuration"
            "set|linear-programming|configuration"
            "set|constraint-solving|configuration"
            "bdd|configuration"
            "bdd|threading|configuration"
    )
    require_same_dependency_commit(Logging
        CHAINS
            "set|linear-programming|algebra|interval|numeric|logging"
            "bdd|threading|logging"
    )
    require_same_dependency_commit(PythonCommon
        CHAINS
            "set|linear-programming|algebra|interval|numeric|foundation|python-common"
            "bdd|python-common"
    )
endif()

setup_project()

include_directories(${PROJECT_SOURCE_DIR}/include)
add_subdirectory(ss)

add_library(paving INTERFACE)
target_include_directories(paving INTERFACE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include/ariadne-paving>
)
target_link_libraries(paving INTERFACE set bdd)

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
