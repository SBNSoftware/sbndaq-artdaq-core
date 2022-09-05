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
    FragmentSerialBase.cc
    BernCRTFragmentSerial.cc
    CAENV1730FragmentSerial.cc
  LIBRARIES
    ${LIBRARY_CORE_LIB_LIST}
    sbndaq-artdaq-core_Overlays_SBND_NevisTPC
    sbndaq-artdaq-core_Overlays
    sbndaq-artdaq-core_Overlays_Common
)

install_headers()
install_source()

