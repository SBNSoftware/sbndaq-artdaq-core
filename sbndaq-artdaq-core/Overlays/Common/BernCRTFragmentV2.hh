#ifndef sbndaq_artdaq_core_Overlays_Common_BernCRTFragmentV2_hh
#define sbndaq_artdaq_core_Overlays_Common_BernCRTFragmentV2_hh

#include "artdaq-core/Data/Fragment.hh"
#include "cetlib_except/exception.h"

#include <iostream>
#include <sstream>
#include <string>

// Implementation of "BernCRTFragmentV2", an artdaq::Fragment overlay class

namespace sbndaq {

  struct BernCRTHitV2;
  std::ostream & operator << (std::ostream &, BernCRTHitV2 const &);

  class BernCRTFragmentV2;
  std::ostream & operator << (std::ostream &, BernCRTFragmentV2 const &);

  class BernCRTFragmentMetadataV2;
  std::ostream & operator << (std::ostream &, BernCRTFragmentMetadataV2 const&);
  
}

class sbndaq::BernCRTFragmentMetadataV2 {

public:

  //get functions
  uint8_t  const& MAC5()             const { return _mac5; }
  uint64_t const& run_start_time()   const { return _run_start_time; }
  uint64_t const& this_poll_start()  const { return _this_poll_start; }
  uint64_t const& this_poll_end()    const { return _this_poll_end; }
  uint64_t const& last_poll_start()  const { return _last_poll_start; }
  uint64_t const& last_poll_end()    const { return _last_poll_end; }
  int32_t  const& system_clock_deviation() const { return _system_clock_deviation; }
  uint32_t const& hits_in_poll()     const { return _hits_in_poll; }
  uint16_t const& hits_in_fragment() const { return _hits_in_fragment; }

  //set functions
  void set_mac5            (int8_t  mac5)      { _mac5 = mac5; }
  void set_clock_deviation (int32_t deviation) { _system_clock_deviation = deviation; }
  void set_hits_in_poll    (uint32_t n)        { _hits_in_poll = n;}
  void set_hits_in_fragment(uint16_t n)        { _hits_in_fragment = n;}
  void set_run_start_time  (uint64_t t)        { _run_start_time = t; }
  
  void update_poll_time(uint64_t poll_start, uint64_t poll_end) {
    /**
     * Copies fields from this poll to the last poll and updates this poll values
     */
    _last_poll_start = _this_poll_start;
    _last_poll_end   = _this_poll_end;
    _this_poll_start = poll_start;
    _this_poll_end   = poll_end;
    if(_last_poll_start == 0) { //educated guess for the first poll in the run
      _last_poll_start = poll_start - 200000000;
      _last_poll_end   = poll_end   - 200000000;
    }
  }
  
  const char* c_str() const { std::ostringstream ss; ss << *this; return ss.str().c_str(); }

private:
  uint8_t   _mac5                    = 0;
  uint64_t  _run_start_time          = 0;
  uint64_t  _this_poll_start         = 0;
  uint64_t  _this_poll_end           = 0;
  uint64_t  _last_poll_start         = 0;
  uint64_t  _last_poll_end           = 0;
  int32_t   _system_clock_deviation  = 0; //system clock deviation w.r.t. steady clock, synchronised at the beginning of the run
  uint32_t  _hits_in_poll            = 0; //includes lost ones //NOTE 16 bits might be sufficient
  uint16_t  _hits_in_fragment        = 0; //size of the fragment hit array
};


struct sbndaq::BernCRTHitV2 {
  uint8_t  flags    = 0;
  uint16_t lostcpu  = 0;
  uint16_t lostfpga = 0; //NOTE: only 8 bits used. We could make it uint8_t
  uint32_t ts0      = 0;
  uint32_t ts1      = 0;
  uint16_t adc[32]  = {0};
//  uint32_t coinc    = 0; //AA Jan 14: commenting out for the purpose of tests at DAB

  //Data added by fragment generator
  uint64_t  feb_hit_number          = 0; //hit counter for individual FEB, including hits lost in FEB or fragment generator
  uint64_t  timestamp               = 0; //absolute timestamp
  uint64_t  last_accepted_timestamp = 0; //timestamp of previous accepted hit
  uint16_t  lost_hits               = 0; //number of lost hits from the previous one

public:
  //get functions
  bool IsOverflow_TS0()  const { return !(flags&1); } //TODO double check the logic
  bool IsOverflow_TS1()  const { return !(flags&2); }
  bool IsReference_TS0() const { return   flags&4; }
  bool IsReference_TS1() const { return   flags&8; }
  
  const char* c_str() const { std::ostringstream ss; ss << *this; return ss.str().c_str(); }
};

class sbndaq::BernCRTFragmentV2 {
public:

  BernCRTFragmentV2(artdaq::Fragment const & f) : artdaq_Fragment_(f) {}

  BernCRTFragmentMetadataV2 const * metadata() const { return artdaq_Fragment_.metadata<BernCRTFragmentMetadataV2>(); }

  BernCRTHitV2 const* eventdata(size_t idx) const {
    if(idx >= metadata()->hits_in_fragment()) {
      throw cet::exception("BernCRTHitOutOfRange") << " Requested Event Index " << std::to_string(idx) << " ≥ " << std::to_string(metadata()->hits_in_fragment());
    }
    return reinterpret_cast<BernCRTHitV2 const*>(artdaq_Fragment_.dataBeginBytes()) + idx;
  }

  size_t DataPayloadSize() const { return artdaq_Fragment_.dataSizeBytes(); }

  const char* c_str() const { std::ostringstream ss; ss << *this; return ss.str().c_str(); }
  

private:

  artdaq::Fragment const & artdaq_Fragment_;

};

#endif /* sbndaq_artdaq_core_Overlays_Common_BernCRTFragmentV2_hh */
