include_directories(${Python3_INCLUDE_DIRS})
get_target_property(PYTHON_COMMON_SOURCE_DIR pyariadne-python-common SOURCE_DIR)

add_library(pyariadne-paving-submodule-obj OBJECT paving_submodule.cpp)
target_include_directories(pyariadne-paving-submodule-obj PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${PYTHON_COMMON_SOURCE_DIR}/include"
    $<TARGET_PROPERTY:pyariadne-set,INTERFACE_INCLUDE_DIRECTORIES>
)
target_link_libraries(pyariadne-paving-submodule-obj PRIVATE
    pyariadne-python-common
    pybind11::module
    paving
)
set_target_properties(pyariadne-paving-submodule-obj PROPERTIES POSITION_INDEPENDENT_CODE ON)
if(MSVC)
    target_compile_options(pyariadne-paving-submodule-obj PRIVATE /bigobj /wd4702)
endif()

add_library(pyariadne-paving INTERFACE)
target_sources(pyariadne-paving INTERFACE $<TARGET_OBJECTS:pyariadne-paving-submodule-obj>)
target_include_directories(pyariadne-paving INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<TARGET_PROPERTY:pyariadne-set,INTERFACE_INCLUDE_DIRECTORIES>
)
target_link_libraries(pyariadne-paving INTERFACE pyariadne-set)
ariadne_add_python_module(NAME paving MODULE_SOURCE paving_module.cpp)
