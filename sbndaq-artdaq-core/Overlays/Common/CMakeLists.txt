cet_make_library( LIBRARY_NAME sbndaq-artdaq-core_Overlays_Common_Utilities
		  SOURCE 
		  Utilities.cc
		  LIBRARIES
		  ${ARTDAQ_DAQDATA}
		  ${ARTDAQ-CORE_UTILITIES}
		  ${CETLIB}
		  ${CETLIB_EXCEPT}
                  ${Boost_SYSTEM_LIBRARY}
                  ${MF_MESSAGELOGGER}
                  ${MF_UTILITIES}
)

cet_make_library( LIBRARY_NAME sbndaq-artdaq-core_Overlays_Common
		  SOURCE 
		  CAENV1730Fragment.cc
		  SpectratimeEventFragment.cc
		  BernCRTZMQFragment.cc
		  BernCRTFragment.cc
		  BernCRTFragmentV2.cc
		  WhiteRabbitFragment.cc
		  DummyFragment.cc
		  BernCRTTranslator.cc
		  LIBRARIES
		  ${ARTDAQ_DAQDATA}
		  ${ARTDAQ-CORE_UTILITIES}
		  ${CETLIB}
		  ${CETLIB_EXCEPT}
#below are lines I copied from icaruscode [aa]
		  art_Framework_Services_Registry
		  art_root_io_tfile_support
		  art_root_io_TFileService_service
		  ${ROOT_BASIC_LIB_LIST}
		  ${ARTDAQ-CORE_UTILITIES}
                  artdaq-core_Data
        cetlib_except
        ${CANVAS}
        ${ART_FRAMEWORK_PRINCIPAL}
        artdaq-core_Data
        art_Framework_Principal_Event
)


install_headers()
install_source()
