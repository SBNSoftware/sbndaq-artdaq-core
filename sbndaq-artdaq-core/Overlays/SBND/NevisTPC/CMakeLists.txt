add_definitions(-Wno-maybe-uninitialized)

cet_make_library(
  LIBRARY_NAME
    sbndaq-artdaq-core_Overlays_SBND_NevisTPC
  SOURCE NevisTPCTypes.cc
    NevisTPCUtilities.cc
  LIBRARIES
    ${LIBRARY_CORE_LIB_LIST}
)

install_headers()
install_source()
