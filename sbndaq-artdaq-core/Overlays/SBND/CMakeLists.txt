add_subdirectory(NevisTPC)

cet_make_library( LIBRARY_NAME sbndaq-artdaq-core_Overlays_SBND
		  SOURCE 
		  PTB_content.cc
		  PTBFragment.cc
		  NevisTPCFragment.cc
		  LIBRARIES
		  sbndaq-artdaq-core_Overlays_SBND_NevisTPC
		  ${ARTDAQ_DAQDATA}
		  artdaq-core_Utilities
		  ${CETLIB}
		  ${CETLIB_EXCEPT}
)


install_headers()
install_source()

