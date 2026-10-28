find_library(ARIADNE_PAVING_LIBRARY NAMES ariadne-paving)

find_package(Threads REQUIRED)

find_path(ARIADNE_PAVING_INCLUDE_DIR ariadne-paving.hpp PATH_SUFFIXES ariadne-paving)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(AriadnePaving DEFAULT_MSG
  ARIADNE_PAVING_LIBRARY
  ARIADNE_PAVING_INCLUDE_DIR
)

if(AriadnePaving_FOUND)
  get_filename_component(ARIADNE_PAVING_INCLUDE_PARENT_DIR ${ARIADNE_PAVING_INCLUDE_DIR} DIRECTORY)
  set(ARIADNE_PAVING_INCLUDE_DIRS
    ${ARIADNE_PAVING_INCLUDE_PARENT_DIR}
    ${ARIADNE_PAVING_INCLUDE_DIR}
  )
  if(NOT TARGET AriadnePaving::ariadne-paving)
    add_library(AriadnePaving::ariadne-paving UNKNOWN IMPORTED)
    set_target_properties(AriadnePaving::ariadne-paving PROPERTIES
      IMPORTED_LOCATION "${ARIADNE_PAVING_LIBRARY}"
      INTERFACE_INCLUDE_DIRECTORIES "${ARIADNE_PAVING_INCLUDE_DIRS}"
      INTERFACE_LINK_LIBRARIES "Threads::Threads"
    )
  endif()
endif()
