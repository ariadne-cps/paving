add_library(PAVING_SRC OBJECT
    grid.cpp
    grid_paving.cpp
    grid_cell_alignment.cpp
    robdd_paving.cpp
    paver.cpp
    set_approximation.cpp
)
target_include_directories(PAVING_SRC PRIVATE "${PROJECT_SOURCE_DIR}/include/paving")
