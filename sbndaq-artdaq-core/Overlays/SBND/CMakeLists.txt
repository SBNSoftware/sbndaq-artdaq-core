add_subdirectory(NevisTPC)

cet_make_library( LIBRARY_NAME sbndaq-artdaq-core_Overlays_SBND
		  SOURCE 
		  PTB_content.cc
		  PTBFragment.cc
		  LIBRARIES
		  ${ARTDAQ_DAQDATA}
		  ${CETLIB}
		  ${CETLIB_EXCEPT}
)


install_headers()
install_source()

