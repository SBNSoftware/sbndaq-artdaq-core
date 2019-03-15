add_subdirectory(NevisTPC)


cet_make_library( LIBRARY_NAME sbndaq-artdaq-core_Overlays_SBND
		  SOURCE 
		  NevisTPCFragment.cc
		  PTBFragment.cc
		  PTB_Content.cc
		  LIBRARIES
		  sbndaq-artdaq-core_Overlays_SBND_NevisTPC
		  ${ARTDAQ_DAQDATA}
		  ${CETLIB}
		  ${CETLIB_EXCEPT}
)

install_headers()
install_source()

