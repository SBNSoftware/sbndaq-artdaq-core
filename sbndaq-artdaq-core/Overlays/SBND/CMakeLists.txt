add_subdirectory(NevisTPC)

cet_make_library(
  LIBRARY_NAME
    sbndaq-artdaq-core_Overlays_SBND
  SOURCE
    PTB_content.cc
    PTBFragment.cc
    NevisTPCFragment.cc
    DAPHNEFragment.cc
  LIBRARIES
    ${LIBRARY_CORE_LIB_LIST}
    sbndaq-artdaq-core_Overlays_SBND_NevisTPC
)

install_headers()
install_source()

