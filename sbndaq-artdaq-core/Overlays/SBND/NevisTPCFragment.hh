#ifndef sbndaq_Overlays_SBND_NevisTPCFragment_hh
#define sbndaq_Overlays_SBND_NevisTPCFragment_hh

#include "artdaq-core/Data/Fragment.hh"
#include "sbndaq-artdaq-core/Overlays/SBND/NevisTPC/NevisTPCTypes.hh"
#include "sbndaq-artdaq-core/Overlays/SBND/NevisTPC/NevisTPCUtilities.hh"
#include "cetlib_except/exception.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

namespace sbndaq{
  
  class NevisTPCFragmentMetadata;
  std::ostream & operator << (std::ostream &, NevisTPCFragmentMetadata const &);
  
  class NevisTPCFragment;
  std::ostream & operator << (std::ostream &, NevisTPCFragment const &);
}

class sbndaq::NevisTPCFragmentMetadata {
  
private:
  uint32_t _event_number;
  uint32_t _samples_per_channel;
  uint32_t _n_channels;
  bool     _is_compressed;
  
public:
  NevisTPCFragmentMetadata(){}
  NevisTPCFragmentMetadata(uint32_t en, uint32_t n_ch, uint32_t n_samples, bool compressed){
    _event_number = en;
    _n_channels = n_ch;
    _samples_per_channel = n_samples;
    _is_compressed = compressed;
  }
  
  uint32_t const& EventNumber()		const { return _event_number; }
  uint32_t const& NChannels()           const { return _n_channels; }
  uint32_t const& SamplesPerChannel()   const { return _samples_per_channel; }
  bool     const& IsCompressed()        const { return _is_compressed; }
  
  void SetEventNumber(uint32_t en) { _event_number = en; }
  void SetNChannels(uint32_t nc) { _n_channels = nc; }
  void SetSamplesPerChannel(uint32_t ns) { _samples_per_channel = ns; }
  void SetIsCompressed(bool c) { _is_compressed = c; }

  size_t ExpectedDataSize() const {
    return (sizeof(NevisTPCHeader)+(sizeof(NevisTPC_ADC_t)*_samples_per_channel*_n_channels));
  }
};

class sbndaq::NevisTPCFragment {
  
public:
  NevisTPCFragment(artdaq::Fragment const & f) :
    artdaq_Fragment_(f)
  {}
  
  NevisTPCFragmentMetadata const * metadata() const
  { return artdaq_Fragment_.metadata<NevisTPCFragmentMetadata>(); }
  
  NevisTPCHeader const * header() const 
  { return reinterpret_cast<NevisTPCHeader const*>(artdaq_Fragment_.dataBeginBytes()); }
  
  NevisTPC_ADC_t const * data() const
  { return reinterpret_cast<uint16_t const*>(artdaq_Fragment_.dataBeginBytes()+sizeof(NevisTPCHeader)); }

  size_t ExpectedDataSize() const
  { return metadata()->ExpectedDataSize(); }

  size_t decode_data(std::unordered_map< uint16_t,NevisTPC_Data_t >& wvfm_map) const
  { 
    NevisTPCDecoder decoder(metadata()->SamplesPerChannel());
    return decoder.decode_data(data(),header()->getADCWordCount(),wvfm_map);
  }

  bool Verify() const;
  
private:
  artdaq::Fragment const & artdaq_Fragment_;
};


#endif
