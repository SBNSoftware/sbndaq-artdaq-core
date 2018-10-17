cet_make_library( LIBRARY_NAME sbndaq-artdaq-core_Overlays_Common
		  SOURCE 
		  CAENV1730Fragment.cc
		  LIBRARIES
		  ${ARTDAQ_DAQDATA}
		  ${CETLIB}
		  ${CETLIB_EXCEPT}
)

install_headers()
install_source()
