cet_make_library(
  LIBRARY_NAME
    sbndaq-artdaq-core_Trace
  SOURCE
    trace_defines.cc
  LIBRARIES PUBLIC
    TRACE::TRACE
)

install_headers()
install_source()

