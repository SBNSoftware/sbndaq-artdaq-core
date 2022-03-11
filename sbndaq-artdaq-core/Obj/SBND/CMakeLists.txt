cet_make_library(
  LIBRARY_NAME
    sbndaq-artdaq-core_Obj_SBND
  SOURCE
    testStandTrigger.cc
  LIBRARIES
    ${LIBRARY_CORE_LIB_LIST}
)

art_dictionary(DICTIONARY_LIBRARIES sbndaq-artdaq-core_Obj_SBND)

install_headers()
install_source()

