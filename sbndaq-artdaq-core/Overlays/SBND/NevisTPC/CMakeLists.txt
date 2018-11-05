cet_make_library( LIBRARY_NAME sbndaq-artdaq-core_Overlays_SBND_NevisTPC
		  SOURCE NevisTPCTypes.cc
		         NevisTPCUtilities.cc
		  LIBRARIES
		    #${ARTDAQ_DAQDATA}
		    #${CETLIB}
		    #${CETLIB_EXCEPT}
		)
install_headers()
install_source()
