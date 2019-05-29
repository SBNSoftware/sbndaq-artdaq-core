#ifndef sbndaq_NevisTPC_NevisTPCUtilities_hh
#define sbndaq_NevisTPC_NevisTPCUtilities_hh

#include "sbndaq-artdaq-core/Overlays/SBND/NevisTPC/NevisTPCTypes.hh"

#include <vector>
#include <unordered_map>

namespace sbndaq{
  class NevisTPCDecoder;  
}

class sbndaq::NevisTPCDecoder {

public:
  NevisTPCDecoder() : fReserveWvfmSize(0) {}
  NevisTPCDecoder(size_t wvfm_size) : fReserveWvfmSize(wvfm_size) {}

  void SetExpectedWaveformSize(size_t wvfm_size) { fReserveWvfmSize = wvfm_size; }

  // return a header object
  NevisTPCHeader decode_header(const char*);

  //fill a map with uncompressed TPC data
  //return the number of channels filled
  size_t decode_data(const NevisTPC_ADC_t*,
		     size_t,
		     std::unordered_map< uint16_t,NevisTPC_Data_t >&);


  
  //char* interface
  size_t decode_data(const char* data_ptr,
		     size_t n_words,
		     std::unordered_map< uint16_t,NevisTPC_Data_t >& wvfm_map)
  {
    return 
      decode_data(reinterpret_cast<NevisTPC_ADC_t const*>(data_ptr),
		  n_words,
		  wvfm_map);
  }
  
  
private:

  size_t fReserveWvfmSize;

  enum {
    TERROR=0,
    TWARNING=1,
    TINFO=2,
    TDEBUG=3,
    TDECODE=4
  };

  NevisTPCWordType_t get_word_type(uint16_t);
  int decode_huffman(int);  
};

#endif
