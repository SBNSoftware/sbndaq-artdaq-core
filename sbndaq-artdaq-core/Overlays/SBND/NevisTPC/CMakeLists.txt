cet_make_library( LIBRARY_NAME sbndaq-artdaq-core_Overlays_SBND_NevisTPC
      SOURCE NevisTPCTypes.cc
      NevisTPCUtilities.cc
      LIBRARIES
      #artdaq_core::artdaq-core_Data
      artdaq_core::artdaq-core_Utilities
      #cetlib
      #cetlib_except
    )
install_headers()
install_source()
