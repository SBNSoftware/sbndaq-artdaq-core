cet_make_library( LIBRARY_NAME sbndaq-artdaq-core_Overlays_ICARUS
		  SOURCE 
		  PhysCrateFragment.cc
		  PhysCrateStatFragment.cc
		  ICARUSTriggerUDPFragment.cc
		  ICARUSPMTGateFragment.cc		  
		  LIBRARIES
		  ${ARTDAQ_DAQDATA}
			${ARTDAQ-CORE_UTILITIES}
		  ${CETLIB}
		  ${CETLIB_EXCEPT}
)

install_headers()
install_source()
