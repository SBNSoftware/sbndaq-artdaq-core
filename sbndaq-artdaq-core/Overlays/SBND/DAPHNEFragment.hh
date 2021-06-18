//
//  sbndaq-artdaq-code/Overlays/SBND/DAPHNEFragment.hh  (W.Badgett)
//
//  Place holder class for DAPHNE fragment definition
//

#ifndef sbndaq_Overlays_SBND_DAPHNEFragment_hh
#define sbndaq_Overlays_SBND_DAPHNEFragment_hh

#include "artdaq-core/Data/Fragment.hh"
#include "cetlib_except/exception.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

namespace sbndaq
{
  class DAPHNEFragmentMetadata;
  std::ostream & operator << (std::ostream &, DAPHNEFragmentMetadata const &);
  
  class DAPHNEFragment;
  std::ostream & operator << (std::ostream &, DAPHNEFragment const &);
}

class sbndaq::DAPHNEFragmentMetadata 
{
public:
  DAPHNEFragmentMetadata(){}
};

class sbndaq::DAPHNEFragment 
{
public:
  DAPHNEFragment(artdaq::Fragment const & f) :
    artdaq_Fragment_(f)
  {}
  
  DAPHNEFragmentMetadata const * metadata() const
  { return artdaq_Fragment_.metadata<DAPHNEFragmentMetadata>(); }
  
  bool Verify() const;
  
private:
  artdaq::Fragment const & artdaq_Fragment_;
};


#endif
