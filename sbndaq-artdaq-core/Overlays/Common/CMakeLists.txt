cet_make_library( LIBRARY_NAME sbndaq-artdaq-core_Overlays_Common_Utilities
      SOURCE 
      Utilities.cc
      LIBRARIES
      artdaq_core::artdaq-core_Data
      artdaq_core::artdaq-core_Utilities
      cetlib
      cetlib_except
      MF_MessageLogger
                  ${Boost_SYSTEM_LIBRARY}
                  ${MF_UTILITIES}
)

cet_make_library( LIBRARY_NAME sbndaq-artdaq-core_Overlays_Common
      SOURCE 
      CAENV1730Fragment.cc
      SpectratimeEventFragment.cc
      BernCRTZMQFragment.cc
      BernCRTFragment.cc
      BernCRTFragmentV2.cc
      WhiteRabbitFragment.cc
      DummyFragment.cc
      LIBRARIES
      artdaq_core::artdaq-core_Data
      artdaq_core::artdaq-core_Utilities
      cetlib
      cetlib_except
)

install_headers()
install_source()
