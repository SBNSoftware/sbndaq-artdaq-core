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

  BernCRTFragmentMetadataV2() {}

  BernCRTFragmentMetadataV2(uint64_t l_run_start_time,
                             uint64_t l_this_poll_start,
                             uint64_t l_this_poll_end,
                             uint64_t l_last_poll_start,
                             uint64_t l_last_poll_end,
                             uint32_t l_system_clock_deviation,
                             uint32_t hits_in_poll,
                             uint32_t hits_in_fragment)
    :
    _run_start_time(l_run_start_time),
    _this_poll_start(l_this_poll_start),
    _this_poll_end(l_this_poll_end),
    _last_poll_start(l_last_poll_start),
    _last_poll_end(l_last_poll_end),
    _system_clock_deviation(l_system_clock_deviation),
    _hits_in_poll(hits_in_poll),
    _hits_in_fragment(hits_in_fragment)
  {}

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
  void set_clock_deviation(int32_t deviation) { _system_clock_deviation = deviation; }
  void set_hits_in_poll(uint32_t n)           { _hits_in_poll = n;}
  void set_run_start_time(uint64_t t)         { _run_start_time = t; }
  
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
  uint32_t  _hits_in_poll            = 0; //number of hits for given FEB in a single febdrv poll, including lost ones //NOTE 16 bits might be sufficient
  uint16_t  _hits_in_fragment        = 0; //size of the hit array
};


struct sbndaq::BernCRTHitV2 {
private:
  //Data read from FEB verbatim:
  uint16_t _flags    = 0; //NOTE: only 4 bits used. We could make it uint8_t
  uint16_t _lostcpu  = 0;
  uint16_t _lostfpga = 0; //NOTE: only 8 bits used. We could make it uint8_t
  uint32_t _ts0      = 0;
  uint32_t _ts1      = 0;
  uint16_t _adc[32]  = {0};
  uint32_t _coinc    = 0; //NOTE: only 16 bits seem to be used. We could make it uint16_t

  //Data added by fragment generator
  uint64_t  _timestamp               = 0; //absolute timestamp
  uint64_t  _feb_hit_number          = 0; //hit counter for individual FEB, including hits lost in FEB or fragment generator
  uint32_t  _omitted_hits            = 0; //number of omitted/lost hits from the previous
  uint64_t  _last_accepted_timestamp = 0; //timestamp of previous accepted hit

public:
  //get functions
  bool IsOverflow_TS0()  const { return !(_flags&1); }
  bool IsOverflow_TS1()  const { return !(_flags&2); }
  bool IsReference_TS0() const { return   _flags&4; }
  bool IsReference_TS1() const { return   _flags&8; }

  uint16_t const& flags()       const { return _flags; }
  uint16_t const& lostcpu()     const { return _lostcpu; }
  uint16_t const& lostfpga()    const { return _lostfpga; }
  uint32_t const& ts0()         const { return _ts0; }
  uint32_t const& ts1()         const { return _ts1; }
  uint16_t const* ADC()         const { return _adc; }
  uint16_t const& ADC(size_t c) const { return _adc[c]; }
  uint32_t const& coinc()       const { return _coinc; }

  uint64_t const& timestamp()       const { return _timestamp; }
  uint64_t const& feb_hit_number()  const { return _feb_hit_number; }
  uint32_t const& omitted_hits()    const { return _omitted_hits; }
  uint64_t const& last_accepted_timestamp() const { return _last_accepted_timestamp; }
  
  //set functions
  void set_timestamp              (uint64_t t) { _timestamp = t; }
  void set_feb_hit_number         (uint64_t t) { _feb_hit_number = t; }
  void set_omitted_hits           (uint32_t n) { _omitted_hits = n; }
  void set_last_accepted_timestamp(uint64_t t) { _last_accepted_timestamp = t; }
  
  const char* c_str() const { std::ostringstream ss; ss << *this; return ss.str().c_str(); }
};

class sbndaq::BernCRTFragmentV2 {
public:

  BernCRTFragmentV2(artdaq::Fragment const & f) : artdaq_Fragment_(f) {}

  BernCRTFragmentMetadataV2 const * metadata() const { return artdaq_Fragment_.metadata<BernCRTFragmentMetadataV2>(); }

  BernCRTHitV2 const* eventdata(size_t idx) const {
    if(idx >= metadata()->hits_in_fragment()) {
      throw cet::exception("CRTHitOutOfRange") << "Requested Event Index " << std::to_string(idx) << " ≥ " << std::to_string(metadata()->hits_in_fragment());
    }
    return reinterpret_cast<BernCRTHitV2 const*>(artdaq_Fragment_.dataBeginBytes()) + idx;
  }

  size_t DataPayloadSize() const { return artdaq_Fragment_.dataSizeBytes(); }

  const char* c_str() const { std::ostringstream ss; ss << *this; return ss.str().c_str(); }
  

private:

  artdaq::Fragment const & artdaq_Fragment_;

};

#endif /* sbndaq_artdaq_core_Overlays_Common_BernCRTFragmentV2_hh */
