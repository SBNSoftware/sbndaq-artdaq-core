#ifndef sbndaq_artdaq_core_Overlays_ICARUS_ICARUSPMTGateFragment_hh
#define sbndaq_artdaq_core_Overlays_ICARUS_ICARUSPMTGateFragment_hh

#include "artdaq-core/Data/Fragment.hh"
#include "cetlib_except/exception.h"

#include <iostream>

namespace icarus {

  class ICARUSPMTGateFragment;

  struct ICARUSPMTGateFragmentMetadata;

}

struct icarus::ICARUSPMTGateFragmentMetadata
{
  std::vector<int> num;

  ICARUSPMTGateFragmentMetadata() {}
  ICARUSPMTGateFragmentMetadata(std::vector<int> n) : num(n) {}

  std::vector<int> getNum() const
  { return num; }
};

class icarus::ICARUSPMTGateFragment{

public:

  ICARUSPMTGateFragment(artdaq::Fragment const &f) : fFragment(f) {}
  ICARUSPMTGateFragmentMetadata const * Metadata() const
  { return fFragment.metadata<ICARUSPMTGateFragmentMetadata>(); }

  size_t DataPayloadSize() const
  { return fFragment.dataSizeBytes(); }

  std::vector<int> getNum() const
  { return Metadata()->getNum(); }

  bool Verify() const;

private:
  artdaq::Fragment fFragment;

};

#endif /* sbndaq_datatypes_Overlays_ICARUSPMTGateFragment_hh */
