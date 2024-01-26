set ( LIBRARY_CORE_LIB_LIST
    artdaq_core::artdaq-core_Data
    artdaq_core::artdaq-core_Utilities
    cetlib::cetlib
    cetlib_except::cetlib_except
)

add_subdirectory(BuildInfo)
add_subdirectory(Trace)
add_subdirectory(Overlays)
add_subdirectory(Obj)
