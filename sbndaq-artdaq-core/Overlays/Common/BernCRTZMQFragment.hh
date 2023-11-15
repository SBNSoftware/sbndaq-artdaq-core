#ifndef sbndaq_artdaq_core_Overlays_Common_BernCRTZMQFragment_hh
#define sbndaq_artdaq_core_Overlays_Common_BernCRTZMQFragment_hh

#include "artdaq-core/Data/Fragment.hh"
#include "cetlib_except/exception.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

//This variable ensures compatibility between data sent by febdrv and received by fragment generator
//Please update it whenever you modify the data format
#define FEBDRV_VERSION 20200120

// Implementation of "BernCRTZMQFragment", an artdaq::Fragment overlay class

namespace sbndaq {

  struct BernCRTZMQEvent;
  std::ostream & operator << (std::ostream &, BernCRTZMQEvent const &);
  
  struct BernCRTZMQLastEvent;

  union BernCRTZMQEventUnion;

  class BernCRTZMQFragment;
  std::ostream & operator << (std::ostream &, BernCRTZMQFragment const &);

  class BernCRTZMQFragmentMetadata;
  std::ostream & operator << (std::ostream &, BernCRTZMQFragmentMetadata const&);

  typedef std::pair<BernCRTZMQEvent,BernCRTZMQFragmentMetadata> BernCRTZMQDataPair;

  double GetCorrectedTime(uint32_t const&, BernCRTZMQFragmentMetadata const&);
}

class sbndaq::BernCRTZMQFragmentMetadata {

public:

  BernCRTZMQFragmentMetadata()
  :
    _run_start_time(0),
    _this_poll_start(0),
    _this_poll_end(0),
    _last_poll_start(0),
    _last_poll_end(0),
    _system_clock_deviation(0),
    _feb_events_per_poll(0),
    _feb_event_number(0),
    _omitted_events(0),
    _last_accepted_timestamp(0)
  {}

  BernCRTZMQFragmentMetadata(uint64_t l_run_start_time,
                             uint64_t l_this_poll_start,
                             uint64_t l_this_poll_end,
                             uint64_t l_last_poll_start,
                             uint64_t l_last_poll_end,
                             uint32_t l_system_clock_deviation,
                             uint32_t feb_c,
                             uint32_t event_n)
    :
    _run_start_time(l_run_start_time),
    _this_poll_start(l_this_poll_start),
    _this_poll_end(l_this_poll_end),
    _last_poll_start(l_last_poll_start),
    _last_poll_end(l_last_poll_end),
    _system_clock_deviation(l_system_clock_deviation),
    _feb_events_per_poll(feb_c),
    _feb_event_number(event_n),
    _omitted_events(0),
    _last_accepted_timestamp(0)
  {}

  uint64_t const& run_start_time()  const { return _run_start_time; }
  uint64_t const& this_poll_start() const { return _this_poll_start; }
  uint64_t const& this_poll_end()   const { return _this_poll_end; }
  uint64_t const& last_poll_start() const { return _last_poll_start; }
  uint64_t const& last_poll_end()   const { return _last_poll_end; }
  int32_t  const& system_clock_deviation() const { return _system_clock_deviation; }
  uint32_t const& feb_events_per_poll()    const { return _feb_events_per_poll; }
  uint32_t const& feb_event_number()       const { return _feb_event_number; }
  
  uint32_t const& omitted_events()          const { return _omitted_events; }
  uint64_t const& last_accepted_timestamp() const { return _last_accepted_timestamp; }

  void set_omitted_events(uint32_t n)          { _omitted_events = n; }
  void set_last_accepted_timestamp(uint64_t t) { _last_accepted_timestamp = t; }
  void set_clock_deviation(int32_t deviation)  { _system_clock_deviation = deviation; }
  void set_feb_events_per_poll(uint32_t n)     { _feb_events_per_poll = n;}
  void set_run_start_time(uint64_t t)          { _run_start_time = t; }
  
  void update_poll_time(uint64_t poll_start, uint64_t poll_end) {
    /**
     * Copies fields from this poll to the last poll and updates this poll values
     */
    _last_poll_start = _this_poll_start;
    _last_poll_end   = _this_poll_end;
    _this_poll_start = poll_start;
    _this_poll_end   = poll_end;
    if(_last_poll_start == 0) {
      //for the very first poll there is no previous poll, yet we need to fill these fields
      _last_poll_start = poll_start - 200000; //200000ns is an example poll period, should be good enough for the very first poll
      _last_poll_end   = poll_end   - 200000;
    }
  }

  uint32_t increment_feb_events(uint32_t n=1)      { _feb_event_number+=n; return feb_event_number(); }
  
  // c14: error: returning address of local temporary object
  // the probolem is  ss.str().c_str()
  const char* c_str() const { std::ostringstream ss; ss << *this; return ss.str().c_str(); }

private:

  uint64_t  _run_start_time;
  uint64_t  _this_poll_start;
  uint64_t  _this_poll_end;
  uint64_t  _last_poll_start;
  uint64_t  _last_poll_end;
  int32_t   _system_clock_deviation;  //system clock deviation w.r.t. steady clock, synchronised at the beginning of the run
  uint32_t  _feb_events_per_poll;     //number of events for given FEB in a single febdrv poll, including lost ones //NOTE 16 bits might be sufficient
  uint32_t  _feb_event_number;        //event counter for individual FEB, taking account events lost in FEB or fragment generator
  uint32_t  _omitted_events;          //number of events omitted by fragment generator
  uint64_t  _last_accepted_timestamp; //if events are omitted, this is timestamp of last accepted event.
                                      //Otherwise it's 0. It's set to 1 if events were omitted at the very beginning of the run

};


struct sbndaq::BernCRTZMQEvent {
  /*
   * Format of data received from febdrv.
   * If this struct ever changes, please update FEBDRV_VERSION
   */

  uint16_t mac5;
  uint16_t flags;    //NOTE: only 4 bits used. We could make it uint8_t
  uint16_t lostcpu;
  uint16_t lostfpga; //NOTE: only 8 bits used. We could make it uint8_t
  uint32_t ts0;
  uint32_t ts1;
  uint16_t adc[32];
  uint32_t coinc;    //NOTE: only 16 bits seem to be used. We could make it uint16_t

  uint16_t const& MAC5() const { return mac5; }
  
  bool     IsOverflow_TS0()  const { return (flags&0x1)==0; }
  bool     IsOverflow_TS1()  const { return (flags&0x2)==0; }
  bool     IsReference_TS0() const { return (flags&0x4)!=0; }
  bool     IsReference_TS1() const { return (flags&0x8)!=0; }
  
  uint32_t const& Time_TS0() const { return ts0; }
  uint32_t const& Time_TS1() const { return ts1; }
  
  uint16_t const* ADC()         const { return adc; }
  uint16_t const& ADC(size_t c) const { return adc[c]; }
  
  const char* c_str() const { std::ostringstream ss; ss << *this; return ss.str().c_str(); }
};

struct sbndaq::BernCRTZMQLastEvent {
  /**
   * Last zeromq event in each zmq packet with a special structure containing number of events and timing information
   * It has the same (or no greater) length as BernCRTZMQEvent, as febdrv sends it as an additional event in the list
   * If this struct ever changes, please update FEBDRV_VERSION
   */
  uint16_t mac5;
  uint16_t flags;
  uint32_t magic_number;
  uint32_t febdrv_version;
  uint32_t n_events;

  //febdrv poll start and end time (ns since the epoch measured by system clock)
  uint64_t poll_time_start;
  uint64_t poll_time_end;

  //deviation of the variables above w.r.t. steady clock synchronised each time febdrv receives DAQ_BEG command
  int32_t poll_start_deviation;
  int32_t poll_end_deviation;

  //variables to match the size of BernCRTZMQEvent.
  //They are not mandatory, but they allow to fill the remainder of the last event with zeros
  uint64_t zero_padding0;
  uint64_t zero_padding1;
  uint64_t zero_padding2;
  uint64_t zero_padding3;
  uint64_t zero_padding4;
  uint32_t zero_padding5;
};

union sbndaq::BernCRTZMQEventUnion {
  /**
   * Union needed to read the last BernCRTZMQEvent as BernCRTZMQLastEvent
   */

  sbndaq::BernCRTZMQEvent     event;
  sbndaq::BernCRTZMQLastEvent last_event;
};

class sbndaq::BernCRTZMQFragment {
public:

  BernCRTZMQFragment(artdaq::Fragment const & f) : artdaq_Fragment_(f) {}

  BernCRTZMQFragmentMetadata const * metadata() const { return artdaq_Fragment_.metadata<BernCRTZMQFragmentMetadata>(); }

  BernCRTZMQEvent const* eventdata() const {
    return ( reinterpret_cast<BernCRTZMQEvent const*>(artdaq_Fragment_.dataBeginBytes()) );
  } 

  size_t DataPayloadSize() const { return artdaq_Fragment_.dataSizeBytes(); }

  bool Verify() const;

  const char* c_str() const { std::ostringstream ss; ss << *this; return ss.str().c_str(); }
  
  static std::string print_timestamp(uint64_t t);

private:

  artdaq::Fragment const & artdaq_Fragment_;

};

/*double sbndaq::GetCorrectedTime( uint32_t const& t, BernCRTZMQFragmentMetadata const& m)
{
  return (double)(t+m.time_offset()) * (1.0 - ((double)(m.time_correction_diff())/1.0e9));
}*/

#endif /* sbndaq_artdaq_core_Overlays_Common_BernCRTZMQFragment_hh */
