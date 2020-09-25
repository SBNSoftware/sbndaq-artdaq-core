#include "sbndaq-artdaq-core/Overlays/Common/BernCRTFragment.hh"
#include <iostream>
#include <bitset>

#include "cetlib_except/exception.h"

std::ostream & sbndaq::operator << (std::ostream & os, BernCRTFragmentMetadata const& m){
  os << "BernCRTFragmentMetadata:"
     << "\n\tRun start time:   " << sbndaq::BernCRTFragment::print_timestamp(m.run_start_time())
     << "\n\tThis poll start:  " << sbndaq::BernCRTFragment::print_timestamp(m.this_poll_start())
     << "\n\tThis poll finish: " << sbndaq::BernCRTFragment::print_timestamp(m.this_poll_end())
     << "\n\tLast poll start:  " << sbndaq::BernCRTFragment::print_timestamp(m.last_poll_start())
     << "\n\tLast poll finish: " << sbndaq::BernCRTFragment::print_timestamp(m.last_poll_end())
     << "\n\tClock deviation:  " << m.system_clock_deviation()<<" ns"
     << "\n\tFEB hits/poll:    " << m.feb_events_per_poll()
     << "\n\t#hit in this FEB: " << m.feb_event_number()
     << "\n\tOmitted hits:     " << m.omitted_events()
     << "\n\tLast timestamp:   " << sbndaq::BernCRTFragment::print_timestamp(m.last_accepted_timestamp());
  os << std::endl;
  return os;
}

std::ostream & sbndaq::operator << (std::ostream & os, BernCRTEvent const & e) {
  os << "\nBernCRTEvent"
     << "\n\tMAC5:        0x" << std::hex << e.MAC5() << std::dec << " (" << e.MAC5() << ")";
  os << "\n\tFlags:       " << std::bitset<16>(e.flags);
  os << "\n\tLostCPU:     " << e.lostcpu;
  os << "\n\tLostFPGA:    " << e.lostfpga;
  os << "\n\tTime1 (TS0): " << sbndaq::BernCRTFragment::print_timestamp(e.Time_TS0());
  os << "\n\tTime2 (TS1): " << sbndaq::BernCRTFragment::print_timestamp(e.Time_TS1());
  os << "\n\t[#ch]: ADC  ";
  for(size_t i_c=0; i_c<32; ++i_c) {
    if(!(i_c % 8)) os<<"\n\t";
    os << " ["<<std::setw(2)<<i_c<<"]: " <<std::setw(4)<< e.ADC(i_c);
  }
  os << "\n\tCoincidence: " << std::bitset<32>(e.coinc);
  os << std::endl;
  return os;
}

 std::ostream & sbndaq::operator << (std::ostream & os, BernCRTFragment const & f) {
  os << "BernCRTFragment: "
     << "\n" << *(f.metadata())
     << *(f.eventdata())
     << std::endl;
  return os;
} 

//AA: TODO try to understand what is the purpose of this function, and either make it work, or delete
/* bool sbndaq::BernCRTFragment::Verify() const {
  bool verified=true;

  if(metadata()->n_events()*sizeof(BernCRTEvent) != DataPayloadSize() )
    verified = false;

  return verified;
    
} */

std::string sbndaq::BernCRTFragment::print_timestamp(uint64_t t) {
  char s[43];
  if(t >= (uint64_t)10*1000*1000*1000) {
    sprintf(s, "%lu %03lu %03lu %03lu.%03lu %03lu %03lu [s.ns]",
      (t/1000000000000000000) %1000,
      (t/1000000000000000)    %1000,
      (t/1000000000000)       %1000,
      (t/1000000000)          %1000,
      (t/1000000)             %1000,
      (t/1000)                %1000,
      (t)                     %1000
    );
  }
  else {
    sprintf(s, "            %lu.%03lu %03lu %03lu [s.ns]",
      (t/1000000000)          %1000,
      (t/1000000)             %1000,
      (t/1000)                %1000,
      (t)                     %1000
    );
  }
  return s;
}

/*
std::string sbndaq::BernCRTEvent::db_entry() const {

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
