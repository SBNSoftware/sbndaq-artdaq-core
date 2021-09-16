add_subdirectory(NevisTPC)

cet_make_library( LIBRARY_NAME sbndaq-artdaq-core_Overlays_SBND
      SOURCE
      PTB_content.cc
      PTBFragment.cc
      NevisTPCFragment.cc
      DAPHNEFragment.cc
      LIBRARIES
      sbndaq-artdaq-core_Overlays_SBND_NevisTPC
      artdaq_core::artdaq-core_Data
      artdaq_core::artdaq-core_Utilities
      cetlib
      cetlib_except
)

install_headers()
install_source()

