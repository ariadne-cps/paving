cmake_minimum_required(VERSION 3.15)

project(Paving VERSION 3.0)

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

require_same_dependency_commit(Build CHAINS
        "build" "set|build" "bdd|build" "tree|build")
require_same_dependency_commit(Foundation CHAINS
        "tree|foundation"
        "set|linear-programming|algebra|interval|numeric|foundation")
require_same_dependency_commit(Logging CHAINS
        "set|linear-programming|logging"
        "bdd|threading|logging")
require_same_dependency_commit(PythonCommon CHAINS
        "set|linear-programming|algebra|interval|numeric|foundation|python-common"
        "bdd|python-common")

ariadne_setup_module(paving)
