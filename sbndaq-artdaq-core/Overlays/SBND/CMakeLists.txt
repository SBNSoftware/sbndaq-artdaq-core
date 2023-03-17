add_subdirectory(NevisTPC)

cet_make_library(
  LIBRARY_NAME
    sbndaq-artdaq-core_Overlays_SBND
  SOURCE
    PTB_content.cc
    PTBFragment.cc
    NevisTPCFragment.cc
    DAPHNEFragment.cc
    TDCTimestampFragment.cc
  LIBRARIES
    ${LIBRARY_CORE_LIB_LIST}
    sbndaq-artdaq-core_Overlays_SBND_NevisTPC
)

cet_make_exec(NAME unitTest
  SOURCE
    unitTest.cc
  LIBRARIES
    fhiclcpp
    cetlib
    cetlib_except
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    MF_MessageLogger
    sbndaq-artdaq_fmctdclib
 )

install_headers()
install_source()

