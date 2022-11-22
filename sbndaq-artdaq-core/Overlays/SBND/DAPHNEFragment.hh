#ifndef sbndaq_Overlays_SBND_DAPHNEFragment_hh
#define sbndaq_Overlays_SBND_DAPHNEFragment_hh

#include "artdaq-core/Data/Fragment.hh"
#include "cetlib_except/exception.h"

#include <iostream>

namespace sbndaq {

  class DAPHNEFragment;
  std::ostream & operator << (std::ostream &, DAPHNEFragment const &);

  struct DAPHNEData;
  std::ostream& operator<<(std::ostream&, DAPHNEData const&);


struct DAPHNEData {

uint16_t DataBlock; // want datablock to be an array?

};

class DAPHNEFragment 
{
public:

  struct Metadata { // includes Header info
    uint32_t nSamples;
  };

  explicit DAPHNEFragment(artdaq::Fragment const& f) : artdaq_Fragment_(f) {}
  Metadata const* metadata() const { return artdaq_Fragment_.metadata<Metadata>(); }

private:
  artdaq::Fragment const& artdaq_Fragment_;
};
} // namespace sbndaq

#endif