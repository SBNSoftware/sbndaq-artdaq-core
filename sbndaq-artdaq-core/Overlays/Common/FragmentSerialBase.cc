#include "sbndaq-artdaq-core/Overlays/Common/FragmentSerialBase.hh"
#include "sbndaq-artdaq-core/Overlays/FragmentType.hh"

std::ostream& sbndaq::operator<<(std::ostream& os, FragmentSerialBase const& serial)
{
  os << '\n'                                                                          
     << "Fragment Type: " 
     << sbndaq::fragmentTypeToString(static_cast<sbndaq::FragmentType>(serial.fragment_type)) 
     << '\n'                                                                
     << "Sequence ID:   " << serial.sequence_id << '\n'
     << "Fragment ID:   " << serial.fragment_id << '\n'
     << "Timestamp:     " << serial.timestamp << '\n'
     << std::endl;
  
  return os;
}

