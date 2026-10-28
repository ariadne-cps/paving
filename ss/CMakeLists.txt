if(NOT TARGET set)
    ensure_submodule_initialized(set)
    add_subdirectory(set)
endif()
