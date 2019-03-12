#ifndef sbndaq_artdaq_core_Overlays_ICARUS_PhysCrateStatFragment_hh
#define sbndaq_artdaq_core_Overlays_ICARUS_PhysCrateStatFragment_hh

#include "artdaq-core/Data/Fragment.hh"
#include "cetlib_except/exception.h"

#include <iostream>
#include <vector>

#include "sbndaq-artdaq-core/Overlays/ICARUS/packs.h"

// Implementation of "PhysCrateFragment", an artdaq::Fragment overlay class
// used for ICARUS DAQ

namespace icarus {
  class PhysCrateStatFragment;
  std::ostream & operator << (std::ostream &, PhysCrateStatFragment const &);

  struct PhysCrateStatBlock;
  std::ostream & operator << (std::ostream &, PhysCrateStatBlock const &);

}

struct icarus::PhysCrateStatBlock {
  
  //'STAT'
  uint32_t token;
  
  //memstat1
  uint32_t fifo_status_1;
  
  uint32_t fifo_status_2 : 6;
  uint32_t unused1       : 26;
  
  uint32_t abstime;
  uint32_t mintime;
  
  //crateid
  uint32_t pkt_fmt_ver : 8;
  uint32_t crate_id    : 24;
  
  uint32_t size;

  PhysCrateStatBlock(){}
  PhysCrateStatBlock( statpack const& );
};

class icarus::PhysCrateStatFragment {

  public:

  PhysCrateStatFragment(artdaq::Fragment const & f) : artdaq_Fragment_(f) {}

  size_t DataPayloadSize() const { return artdaq_Fragment_.dataSizeBytes(); }

  PhysCrateStatBlock const * StatBlock() const {
    return ( reinterpret_cast< PhysCrateStatBlock const *>(artdaq_Fragment_.dataBeginBytes()) );
  }

  bool Verify() const;

private:

  artdaq::Fragment const & artdaq_Fragment_;

};


#endif /* sbndaq_artdaq_core_Overlays_ToyFragment_hh */
