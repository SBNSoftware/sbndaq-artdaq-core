#ifndef sbndaq_artdaq_Overlays_Common_DummyFragment_hh
#define sbndaq_artdaq_Overlays_Common_DummyFragment_hh

#include "artdaq-core/Data/Fragment.hh"

#include <ostream>

namespace sbndaq {
class DummyFragment;

}  // namespace sbndaq

class sbndaq::DummyFragment
{
  public:
    struct Metadata {
      uint64_t fragment_fill_time_;
    };
};

#endif /* sbndaq_artdaq_Overlays_Common_DummyFragment_hh */
