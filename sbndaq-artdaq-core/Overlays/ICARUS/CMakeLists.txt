cet_make_library(
  LIBRARY_NAME
    sbndaq-artdaq-core_Overlays_ICARUS
  SOURCE
    PhysCrateFragment.cc
    PhysCrateStatFragment.cc
    ICARUSTriggerUDPFragment.cc
    ICARUSPMTGateFragment.cc
    ICARUSTriggerV2Fragment.cc
    ICARUSTriggerV3Fragment.cc
  LIBRARIES
    ${LIBRARY_CORE_LIB_LIST}
)

install_headers()
install_source()
