
cet_make_library( LIBRARY_NAME sbndaq-artdaq-core_Overlays_ICARUS
      SOURCE 
      PhysCrateFragment.cc
      PhysCrateStatFragment.cc
      ICARUSTriggerUDPFragment.cc
      ICARUSPMTGateFragment.cc      
      LIBRARIES
      artdaq_core::artdaq-core_Data
      artdaq_core::artdaq-core_Utilities
      cetlib
      cetlib_except
)

install_headers()
install_source()
