#ifndef sbndaq_artdaq_core_Overlays_Common_BernCRTZMQFragment_hh
#define sbndaq_Overlays_BernZMQFragment_hh

#include "artdaq-core/Data/Fragment.hh"
#include "cetlib_except/exception.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

//#include "bernfebdrv/febevt.h"

// Implementation of "BernCRTZMQFragment", an artdaq::Fragment overlay class

namespace sbndaq {

  struct BernCRTZMQEvent;
  std::ostream & operator << (std::ostream &, BernCRTZMQEvent const &);

  class BernCRTZMQFragment;
  std::ostream & operator << (std::ostream &, BernCRTZMQFragment const &);

  class BernCRTZMQFragmentMetadata;
  std::ostream & operator << (std::ostream &, BernCRTZMQFragmentMetadata const&);

  typedef std::vector<BernCRTZMQEvent> BernCRTZMQEvents;
  typedef std::pair<BernCRTZMQFragmentMetadata,BernCRTZMQEvents> BernCRTZMQDataPair;

  double GetCorrectedTime(uint32_t const&, BernCRTZMQFragmentMetadata const&);
}

class sbndaq::BernCRTZMQFragmentMetadata {

public:

  BernCRTZMQFragmentMetadata(){}

  BernCRTZMQFragmentMetadata(uint32_t ts_s, uint32_t ts_ns, 
			     uint32_t te_s, uint32_t te_ns,
			     int t_c, uint64_t t_o,
			     uint32_t r, uint32_t seq,
			     uint64_t fid, uint32_t rid,
			     uint32_t nch, uint32_t nadc)
    :
    _time_start_seconds(ts_s),
    _time_start_nanosec(ts_ns),
    _time_end_seconds(te_s),
    _time_end_nanosec(te_ns),
    _time_correction_diff(t_c),
    _time_offset(t_o),
    _run_number(r),
    _sequence_number(seq),
    _feb_id(fid),
    _reader_id(rid),
    _n_channels(nch),
    _n_adc_bits(nadc),
    _missed_events(0),
    _overwritten_events(0),
    _dropped_events(0),
    _n_events(0),
    _n_datagrams(0)    
  { CheckTimeWindow(); }

  uint32_t const& time_start_seconds() const { return _time_start_seconds; }
  uint32_t const& time_start_nanosec() const { return _time_start_nanosec; }
  
  uint32_t const& time_end_seconds() const { return _time_end_seconds; }
  uint32_t const& time_end_nanosec() const { return _time_end_nanosec; }
  
  int      const& time_correction_diff() const { return _time_correction_diff; }
  uint64_t const& time_offset() const { return _time_offset; }
  
  uint32_t const& run_number()         const { return _run_number; }
  uint32_t const& sequence_number()    const { return _sequence_number; }
  uint64_t const& feb_id()             const { return _feb_id; }
  uint32_t const& reader_id()          const { return _reader_id; }
  uint32_t const& n_channels()         const { return _n_channels; }
  uint32_t const& n_adc_bits()         const { return _n_adc_bits; }
  uint32_t const& missed_events()      const { return _missed_events; }
  uint32_t const& overwritten_events() const { return _overwritten_events; }
  uint32_t const& dropped_events()     const { return _dropped_events; }
  uint32_t const& n_events()           const { return _n_events; }
  uint32_t const& n_datagrams()        const { return _n_datagrams; }

  uint32_t inc_MissedEvents(uint32_t n=1)      { _missed_events+=n; return missed_events(); }
  uint32_t inc_OverwrittenEvents(uint32_t n=1) { _overwritten_events+=n; return overwritten_events(); }
  uint32_t inc_DroppedEvents(uint32_t n=1)     { _dropped_events+=n; return dropped_events(); }
  uint32_t inc_Events(uint32_t n=1)            { _n_events+=n; return n_events(); }
  uint32_t inc_Datagrams(uint32_t n=1)         { _n_datagrams+=n; return n_datagrams(); }
  
  void increment(uint32_t missed, uint32_t overwritten, uint32_t dropped, uint32_t events=1, uint32_t d=0)
  { inc_MissedEvents(missed); inc_OverwrittenEvents(overwritten); inc_DroppedEvents(dropped); inc_Events(events); inc_Datagrams(d); }
  
  const char* c_str() const { std::ostringstream ss; ss << *this; return ss.str().c_str(); }

private:

  uint32_t _time_start_seconds; //time at fragment start, seconds
  uint32_t _time_start_nanosec; //time at fragment start, nanoseconds
  uint32_t _time_end_seconds; //time at fragment end, seconds
  uint32_t _time_end_nanosec; //time at fragment end, nanoseconds

  int _time_correction_diff;
  uint64_t _time_offset;

  uint32_t _run_number;
  uint32_t _sequence_number;

  uint64_t _feb_id; //mac address
  uint32_t _reader_id; //pc ID

  uint32_t _n_channels;
  uint32_t _n_adc_bits;

  uint32_t _missed_events;
  uint32_t _overwritten_events;
  uint32_t _dropped_events;
  uint32_t _n_events;
  uint32_t _n_datagrams;

  void CheckTimeWindow() const;
  
};

struct sbndaq::BernCRTZMQEvent{    

  //typedef struct {
  uint16_t mac5;
  uint16_t flags;
  uint16_t lostcpu;
  uint16_t lostfpga;
  uint32_t ts0;
  uint32_t ts1;
  uint16_t adc[32];
  //} EVENT_t; 
  //
  //EVENT_t event_data;
  
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
  //std::string db_entry() const;
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

private:

  artdaq::Fragment const & artdaq_Fragment_;

};

double sbndaq::GetCorrectedTime( uint32_t const& t, BernCRTZMQFragmentMetadata const& m)
{
  return (double)(t+m.time_offset()) * (1.0 - ((double)(m.time_correction_diff())/1.0e9));
}

#endif /* artdaq_demo_Overlays_ToyFragment_hh */
