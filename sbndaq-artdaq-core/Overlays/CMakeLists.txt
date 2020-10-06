cet_make_library( LIBRARY_NAME sbndaq-artdaq-core_Overlays
		  SOURCE 
		  FragmentType.cc
		  LIBRARIES
		  ${ARTDAQ_DAQDATA}
		  ${CETLIB}
		  ${CETLIB_EXCEPT}
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
