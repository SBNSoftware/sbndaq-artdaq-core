add_definitions(-Wno-nested-anon-types)

IF(SBND_BUILD)
  add_subdirectory(SBND)
ENDIF()

install_headers()
install_source()
