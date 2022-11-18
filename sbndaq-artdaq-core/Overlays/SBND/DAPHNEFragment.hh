#ifndef sbndaq_Overlays_SBND_DAPHNEFragment_hh
#define sbndaq_Overlays_SBND_DAPHNEFragment_hh

#include "artdaq-core/Data/Fragment.hh"
#include "cetlib_except/exception.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

namespace sbndaq {
  class DAPHNEFragment;
  std::ostream & operator << (std::ostream &, DAPHNEFragment const &);

} 

class sbndaq::DAPHNEFragment 
{
public:
  struct Metadata {
    uint64_t fragment_fill_time_;
  };
};


#endif