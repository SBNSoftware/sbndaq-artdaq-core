#include "sbndaq-artdaq-core/Overlays/Common/BernCRTFragmentV2.hh"
#include "sbndaq-artdaq-core/Overlays/Common/BernCRTFragment.hh" //defines print_timestamp
#include <iostream>
#include <bitset>

#include "cetlib_except/exception.h"

std::ostream & sbndaq::operator << (std::ostream & os, BernCRTFragmentMetadataV2 const& m){
  os << "BernCRTFragmentMetadataV2:"
     << "\n\tMAC5:              0x" << std::hex << (int)m.MAC5() << std::dec << " (" << (int)m.MAC5() << ")"
     << "\n\tRun start time:    " << sbndaq::BernCRTFragment::print_timestamp(m.run_start_time())
     << "\n\tThis poll start:   " << sbndaq::BernCRTFragment::print_timestamp(m.this_poll_start())
     << "\n\tThis poll finish:  " << sbndaq::BernCRTFragment::print_timestamp(m.this_poll_end())
     << "\n\tLast poll start:   " << sbndaq::BernCRTFragment::print_timestamp(m.last_poll_start())
     << "\n\tLast poll finish:  " << sbndaq::BernCRTFragment::print_timestamp(m.last_poll_end())
     << "\n\tClock deviation:   " << m.system_clock_deviation()<<" ns"
     << "\n\tFEB hits/poll:     " << m.hits_in_poll()
     << "\n\tFEB hits/fragment: " << m.hits_in_fragment();
  os << std::endl;
  return os;
}

std::ostream & sbndaq::operator << (std::ostream & os, BernCRTHitV2 const & h) {
  os << "BernCRTHitV2:"
     << "\n\tFlags:            " << std::bitset<8>(h.flags)
     <<(h.IsOverflow_TS0() ?" [T0 overflow]" :"")
     <<(h.IsOverflow_TS1() ?" [T1 overflow]" :"")
     <<(h.IsReference_TS0()?" [T0 reference]":"")
     <<(h.IsReference_TS1()?" [T1 reference]":"") 
     << "\n\tLostCPU:          " << h.lostcpu
     << "\n\tLostFPGA:         " << h.lostfpga
     << "\n\tTime1 (TS0):      " << sbndaq::BernCRTFragment::print_timestamp(h.ts0)
     << "\n\tTime2 (TS1):      " << sbndaq::BernCRTFragment::print_timestamp(h.ts1)
     << "\n\t[#ch]: ADC  ";
  for(size_t i_c=0; i_c<32; ++i_c) {
    if(!(i_c % 8)) os<<"\n\t";
    os << " ["<<std::setw(2)<<i_c<<"]: " <<std::setw(4)<< h.adc[i_c];
  }
  os
//     << "\n\tCoincidence:      " << std::bitset<32>(h.coinc)
     << "\n\tTimestamp:        " << sbndaq::BernCRTFragment::print_timestamp(h.timestamp)
     << "\n\tFEB hit number:   " << h.feb_hit_number
     << "\n\tLost hits:        " << h.lost_hits
     << "\n\tLast timestamp:   " << sbndaq::BernCRTFragment::print_timestamp(h.last_accepted_timestamp)
     << std::endl;
  return os;
}

 std::ostream & sbndaq::operator << (std::ostream & os, BernCRTFragmentV2 const & f) {
  os << "BernCRTFragmentV2: "
     << "\n" << *(f.metadata());
  //Loop over all hits in the fragment
  for(int idx = 0; idx < f.metadata()->hits_in_fragment(); idx++) {
    os << "("<<std::to_string(idx+1) <<"/"<<f.metadata()->hits_in_fragment()<< ") "
       << *(f.eventdata(idx));
  }
  return os;
} 


