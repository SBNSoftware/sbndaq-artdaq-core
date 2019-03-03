#ifndef sbndaq_artdaq_core_Overlays_FragmentType_hh
#define sbndaq_artdaq_core_Overlays_FragmentType_hh
#include "artdaq-core/Data/Fragment.hh"

namespace sbndaq {

  namespace detail {
    enum FragmentType : artdaq::Fragment::type_t
    { MISSED = artdaq::Fragment::FirstUserFragmentType,
	//COMMON
	CAENV1730,
	SpectratimeEvent,
	BERNCRTZMQ,

	//ICARUS
	PHYSCRATEDATA,
	PHYSCRATESTAT,

	//SBND
	NevisTPC,
	PENNTRIGGER,
	TRIGGERBOARD,

        INVALID // Should always be last.
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
