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

  typedef uint64_t data_t;
  int numFEBs; // need to know number of FEBs connected to loop through them

  struct DAPHNEHeader {
    uint16_t wordCount;
    uint64_t microbunch : 48;
    uint32_t timestamp;
    uint32_t customWords;
  } hdr;

  struct DAPHNE_FEB_Channel {

    uint16_t channel;
    std::array<uint16_t,14> waveform;

  } febch;

  struct DAPHNE_FEB_Header {

    uint16_t wordCount;
    uint16_t statusWord1;
    uint32_t microbunch;
    uint16_t emptyWord;
    uint16_t statusWord2;
    
  } febhdr;

  struct DAPHNE_FEB_Fragment {

    DAPHNE_FEB_Header febHeader;
    std::array<DAPHNE_FEB_Channel,16> channels;

  } febfrg;

  struct DAPHNEFragment {

    DAPHNEHeader header;
    std::vector<DAPHNE_FEB_Fragment> febFrags;

  } frag;

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