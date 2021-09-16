add_definitions(-Wno-nested-anon-types)

cet_make_library(
  LIBRARY_NAME
    sbndaq-artdaq-core_Overlays
  SOURCE
    FragmentType.cc
  LIBRARIES
    ${LIBRARY_CORE_LIB_LIST}
)

add_subdirectory(Common)

IF(ICARUS_BUILD)
  add_subdirectory(ICARUS)
ENDIF()

IF(SBND_BUILD)
  add_subdirectory(SBND)
ENDIF()

install_headers()
install_source()
