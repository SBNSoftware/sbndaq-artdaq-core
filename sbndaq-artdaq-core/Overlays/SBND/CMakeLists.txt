add_subdirectory(NevisTPC)

cet_make_library( LIBRARY_NAME sbndaq-artdaq-core_Overlays_SBND_Penn
		  SOURCE 
                  PennMicroSlice.cc
                  PennMicroSliceWriter.cc
                  PennMilliSlice.cc
                  PennMilliSliceWriter.cc
		  LIBRARIES
		  ${ARTDAQ_DAQDATA}
		  ${CETLIB}
		  ${CETLIB_EXCEPT}
                  ${Boost_SYSTEM_LIBRARY}
                  ${MF_MESSAGELOGGER}
                  ${MF_UTILITIES}
)

cet_make_library( LIBRARY_NAME sbndaq-artdaq-core_Overlays_SBND
		  SOURCE 
		  NevisTPCFragment.cc
		  PennMilliSliceFragment.cc
		  LIBRARIES
		  sbndaq-artdaq-core_Overlays_SBND_NevisTPC
		  sbndaq-artdaq-core_Overlays_SBND_Penn
		  ${ARTDAQ_DAQDATA}
		  ${CETLIB}
		  ${CETLIB_EXCEPT}
)

install_headers()
install_source()

