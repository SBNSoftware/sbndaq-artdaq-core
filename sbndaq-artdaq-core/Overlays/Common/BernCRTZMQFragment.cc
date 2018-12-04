#include "sbndaq-artdaq-core/Overlays/Common/BernCRTZMQFragment.hh"
#include <iostream>

#include "cetlib/exception.h"

void bernfebdaq::BernCRTZMQFragmentMetadata::CheckTimeWindow() const {
  if(_time_end_seconds*1e9+_time_end_nanosec < _time_start_seconds*1e9+_time_start_nanosec)
    throw cet::exception("BernFragmentMetadata::CheckTimeWindow") 
      << "Failed. time_start (" << _time_start_seconds << "," << _time_start_nanosec 
      << ") is after time_end (" << _time_end_seconds << "," << _time_end_nanosec  << ")";
}


std::ostream & bernfebdaq::operator << (std::ostream & os, BernCRTZMQFragmentMetadata const& m){
  os << "\nBernCRTZMQFragmentMetadata:"
     << "\n\tTime window start: " << m.time_start_seconds() << " s, " << m.time_start_nanosec() << " ns" 
     << "\n\tTime window end: " << m.time_end_seconds() << " s, " << m.time_end_nanosec() << " ns" 
     << "\n\tRaw time correction factor: " << m.time_correction_diff()
     << "\n\tRaw time offset: " << m.time_offset()
     << "\n\tRun number: " << m.run_number()
     << "\n\tSequence number: " << m.sequence_number()
     << std::hex
     << "\n\tZMQ ID: " << m.feb_id() 
     << "\n\tReader ID: " << m.reader_id()
     << std::dec
     << "\n\tNumber of channels: " << m.n_channels()
     << "\n\tNumber of adc bits: " << m.n_adc_bits()
     << "\n\tNumber of missed events: " << m.missed_events()
     << "\n\tNumber of overwritten events: " << m.overwritten_events()
     << "\n\tNumber of dropped events: " << m.dropped_events()
     << "\n\tNumber of events recorded: " << m.n_events()
     << "\n\tNumber of datagrams: " << m.n_datagrams();
  os << std::endl;
  return os;
}

std::ostream & bernfebdaq::operator << (std::ostream & os, BernCRTZMQEvent const & e){
  os << "\nBernCRTZMQEvent"
     << "\n\tMAC5: " << std::hex << e.MAC5() << std::dec;
  os << "\n\tFlags word: " << std::hex << e.flags << std::dec;
  os << "\n\tLostCPU: " << e.lostcpu;
  os << "\n\tLostFPGA: " << e.lostfpga;
  os << "\n\tTime1 (TS0): " << e.Time_TS0();
  os << "\n\tTime2 (TS1): " << e.Time_TS1();
  os << std::hex;
  for(size_t i_c=0; i_c<32; ++i_c){
    if(i_c%8==0) os << "\n\t\t";
    os << "0x" << e.ADC(i_c) << " ";
  }
  os << std::dec;
  os << std::endl;
  return os;
}

std::ostream & bernfebdaq::operator << (std::ostream & os, BernCRTZMQFragment const & f) {
  os << "BernCRTZMQFragment: "
     << "\n" << *(f.metadata());
  for(size_t i_b=0; i_b<f.metadata()->n_events(); ++i_b)
    os << "\nEvent " << i_b
       << *(f.eventdata(i_b));
  os << std::endl;
  return os;
}

bool bernfebdaq::BernCRTZMQFragment::Verify() const {
  bool verified=true;

  if(metadata()->n_events()*sizeof(BernCRTZMQEvent) != DataPayloadSize() )
    verified = false;

  return verified;
    
}
/*
std::string bernfebdaq::BernCRTZMQEvent::db_entry() const {

  std::stringstream ss_tags,ss_data;
  ss_tags << "time1_ref=" << time1.IsReference() << ",";
  ss_tags << "time2_ref=" << time2.IsReference() << ",";
  ss_tags << "time1_ovfl=" << time1.IsOverflow() << ",";
  ss_tags << "time2_ovfl=" << time2.IsOverflow() << ",";

  ss_data << "overwritten=" << flags.overwritten << ",";
  ss_data << "missed=" << flags.missed << ",";
  ss_data << "time1=" << time1.Time() << ",";
  ss_data << "time2=" << time2.Time() << ",";
  for(size_t i=0; i<32; ++i)
    ss_data << "ch" << i << "=" << adc[i] << ",";


  return (ss_tags.str()+" "+ss_data.str());
}
*/
