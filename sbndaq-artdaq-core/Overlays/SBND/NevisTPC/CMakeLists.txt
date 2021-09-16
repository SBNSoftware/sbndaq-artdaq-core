cet_make_library( LIBRARY_NAME sbndaq-artdaq-core_Overlays_SBND_NevisTPC
      SOURCE NevisTPCTypes.cc
      NevisTPCUtilities.cc
      LIBRARIES
      artdaq_core::artdaq-core_Utilities
    )

install_headers()
install_source()
