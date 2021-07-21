add_definitions(-Wno-nested-anon-types)
cet_make_library( LIBRARY_NAME sbndaq-artdaq-core_Overlays
      SOURCE 
      FragmentType.cc
      LIBRARIES
      artdaq_core::artdaq-core_Data
      artdaq_core::artdaq-core_Utilities
      cetlib
      cetlib_except
)

install_headers()
install_source()

add_subdirectory(Common)

#IF(ICARUS_BUILD)
add_subdirectory(ICARUS)
#ENDIF()

# Wes will want to fix the following
#IF(SBND_BUILD)
add_subdirectory(SBND)
#ENDIF()
