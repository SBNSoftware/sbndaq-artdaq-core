#ifndef sbndaq_artdaq_core_Overlays_Common_BernCRTZMQFragment_hh
#define sbndaq_Overlays_BernZMQFragment_hh

#include "artdaq-core/Data/Fragment.hh"
#include "cetlib_except/exception.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

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

  BernCRTZMQFragmentMetadata(){}

  BernCRTZMQFragmentMetadata(uint64_t l_run_start_time,
			     uint64_t l_this_poll_start,
   			     uint64_t l_this_poll_end,
			     uint64_t l_last_poll_start,
			     uint64_t l_last_poll_end,
                             uint32_t l_system_clock_deviation,
			     uint32_t feb_c,
			     uint32_t evpack, uint32_t seq)
    :
    _run_start_time(l_run_start_time),
    _this_poll_start(l_this_poll_start),
    _this_poll_end(l_this_poll_end),
    _last_poll_start(l_last_poll_start),
    _last_poll_end(l_last_poll_end),
    _system_clock_deviation(l_system_clock_deviation),
    _feb_event_count(feb_c),
    _events_in_data_packet(evpack),
    _sequence_number(seq),
    _missed_events(0),
    _overwritten_events(0),
    _dropped_events(0),
    _n_events(0),
    _n_datagrams(0)
  {}

  uint64_t const& run_start_time()  const { return _run_start_time; }
  uint64_t const& this_poll_start() const { return _this_poll_start; }
  uint64_t const& this_poll_end()   const { return _this_poll_end; }
  uint64_t const& last_poll_start() const { return _last_poll_start; }
  uint64_t const& last_poll_end()   const { return _last_poll_end; }
  int32_t  const& system_clock_deviation() const { return _system_clock_deviation; }

  uint32_t const& feb_event_count()    const { return _feb_event_count; }
  uint32_t const& event_packet()       const { return _events_in_data_packet; }
  uint32_t const& sequence_number()    const { return _sequence_number;}
  //AA: TODO consider removing these fields
  uint32_t const& missed_events()      const { return _missed_events; }
  uint32_t const& overwritten_events() const { return _overwritten_events; }
  uint32_t const& dropped_events()     const { return _dropped_events; }
  uint32_t const& n_events()           const { return _n_events; }
  uint32_t const& n_datagrams()        const { return _n_datagrams; }

  uint32_t inc_SequenceNumber(uint32_t n=1)    { _sequence_number+=n; return sequence_number(); }
  uint32_t inc_MissedEvents(uint32_t n=1)      { _missed_events+=n; return missed_events(); }
  uint32_t inc_OverwrittenEvents(uint32_t n=1) { _overwritten_events+=n; return overwritten_events(); }
  uint32_t inc_DroppedEvents(uint32_t n=1)     { _dropped_events+=n; return dropped_events(); }
  uint32_t inc_Events(uint32_t n=1)            { _n_events+=n; return n_events(); }
  uint32_t inc_Datagrams(uint32_t n=1)         { _n_datagrams+=n; return n_datagrams(); }
  
  void increment(uint32_t missed, uint32_t overwritten, uint32_t dropped, uint32_t events=1, uint32_t d=0)
  { inc_MissedEvents(missed); inc_OverwrittenEvents(overwritten); inc_DroppedEvents(dropped); inc_Events(events); inc_Datagrams(d); }
  
  const char* c_str() const { std::ostringstream ss; ss << *this; return ss.str().c_str(); }

private:

  uint64_t  _run_start_time;
  uint64_t  _this_poll_start;
  uint64_t  _this_poll_end;
  uint64_t  _last_poll_start;
  uint64_t  _last_poll_end;
  int32_t   _system_clock_deviation; //system clock deviation w.r.t. steady clock, synchronised at the beginning of the run
  //AA: TODO consider removing these
  uint32_t  _feb_event_count;
  uint32_t  _events_in_data_packet;
  uint32_t  _sequence_number;

  uint32_t _missed_events;
  uint32_t _overwritten_events;
  uint32_t _dropped_events;
  uint32_t _n_events;
  uint32_t _n_datagrams;

};


struct sbndaq::BernCRTZMQEvent {

  uint16_t mac5;
  uint16_t flags;
  uint16_t lostcpu;
  uint16_t lostfpga;
  uint32_t ts0;
  uint32_t ts1;
  uint16_t adc[32];
  uint32_t coinc;

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
   */
  uint16_t mac5;
  uint16_t flags;
  uint32_t magic_number0;
  uint32_t magic_number1;
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

  BernCRTZMQEvent const* eventdata(uint16_t e) const {
    if(e > metadata()->n_events())
      throw cet::exception("BernCRTZMQFragment::BernCRTZMQEvent")
	<< "Event requested (" << (uint32_t)e << ") is out of range: " << metadata()->n_events();
    return ( reinterpret_cast<BernCRTZMQEvent const*>(artdaq_Fragment_.dataBeginBytes() + e*sizeof(BernCRTZMQEvent)) );
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

#endif /* artdaq_demo_Overlays_ToyFragment_hh */
