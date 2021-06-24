add_subdirectory(NevisTPC)

cet_make_library( LIBRARY_NAME sbndaq-artdaq-core_Overlays_SBND
		  SOURCE 
		  PTB_content.cc
		  PTBFragment.cc
		  NevisTPCFragment.cc
		  DAPHNEFragment.cc
		  LIBRARIES
		  sbndaq-artdaq-core_Overlays_SBND_NevisTPC
		  ${ARTDAQ_DAQDATA}
		  ${ARTDAQ-CORE_UTILITIES}
		  ${CETLIB}
		  ${CETLIB_EXCEPT}
)


install_headers()
install_source()

