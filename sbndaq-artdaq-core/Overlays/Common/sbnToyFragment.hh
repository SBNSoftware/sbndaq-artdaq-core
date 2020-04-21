#ifndef sbndaq_artdaq_Overlays_Common_sbnToyFragment_hh
#define sbndaq_artdaq_Overlays_Common_sbnToyFragment_hh

#include "artdaq-core/Data/Fragment.hh"

#include <ostream>

namespace sbndaq {
class sbnToyFragment;

/// Let the "<<" operator dump the ToyFragment's data to stdout
std::ostream& operator<<(std::ostream&, sbnToyFragment const&);
}  // namespace sbndaq

class sbndaq::sbnToyFragment
{
  public:
    struct Metadata
    {
      uint64_t fragment_fill_time_;
    };

    struct Header
    {
      int32_t seven_;
    }; 
};

#endif /* sbndaq_artdaq_Overlays_Common_sbnToyFragment_hh */
