if(NOT TARGET set)
    ariadne_add_submodule(set)
endif()

if(NOT TARGET bdd)
    ariadne_add_submodule(bdd)
endif()

if(NOT TARGET tree)
    ariadne_add_submodule(tree)
endif()

ariadne_setup_testing()
