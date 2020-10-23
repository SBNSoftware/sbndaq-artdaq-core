#ifndef sbndaq_artdaq_core_Overlays_FragmentType_hh
#define sbndaq_artdaq_core_Overlays_FragmentType_hh
#include "artdaq-core/Data/Fragment.hh"

namespace sbndaq {

/*
   Note, if you add new frament types, the use a new enum number for them, so that the old numbering scheme is not changed

   */

  namespace detail {
    enum FragmentType : artdaq::Fragment::type_t
    { MISSED = artdaq::Fragment::FirstUserFragmentType,
	//COMMON
	CAENV1730 = artdaq::Fragment::FirstUserFragmentType + 1,
	SpectratimeEvent = artdaq::Fragment::FirstUserFragmentType + 2,
	BERNCRT = artdaq::Fragment::FirstUserFragmentType + 9,
	BERNCRTZMQ = artdaq::Fragment::FirstUserFragmentType + 3,

	//ICARUS
	PHYSCRATEDATA = artdaq::Fragment::FirstUserFragmentType + 4,
	PHYSCRATESTAT = artdaq::Fragment::FirstUserFragmentType + 5,
	ICARUSTriggerUDP = artdaq::Fragment::FirstUserFragmentType + 10,
	ICARUSPMTGate = artdaq::Fragment::FirstUserFragmentType + 11,

	//SBND
	NevisTPC = artdaq::Fragment::FirstUserFragmentType + 6,
	PTB = artdaq::Fragment::FirstUserFragmentType + 7,
	
	//Simulators
	DummyGenerator = artdaq::Fragment::FirstUserFragmentType + 8,

        INVALID = artdaq::Fragment::FirstUserFragmentType + 12 // Should always be last.
        };

    // Safety check.
    static_assert(artdaq::Fragment::isUserFragmentType(FragmentType::INVALID - 1),
                  "Too many user-defined fragments!");
  }

  using detail::FragmentType;

  FragmentType toFragmentType(std::string t_string);
  std::string fragmentTypeToString(FragmentType val);
  
  std::map<artdaq::Fragment::type_t, std::string> makeFragmentTypeMap();

}
#endif /* sbndaq_artdaq_core_Overlays_FragmentType_hh */
