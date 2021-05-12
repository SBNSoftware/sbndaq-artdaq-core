#include "sbndaq-artdaq-core/Overlays/ICARUS/ICARUSTriggerUDPFragment.hh"
#include "cetlib_except/exception.h"



std::ostream & icarus::operator << (std::ostream & os, ICARUSTriggerUDPFragment const & f) {
 
  os << "ICARUSTriggerUDPFragment: " << f.GetDataString();
  os << "\n\t Event Number: " << f.getWRName();
  os << "\n\t Seconds since UTC epoch: " << f.getWRSeconds();
  os << "\n\t Nanoseconds: " << f.getWRNanoSeconds();
  os << "\n\t Gate ID: " << f.getGateID();
  os << "\n\t Gate Type: " << f.getGateType();
  os << "\n\t isBNB: " << f.isBNB();
  os << "\n\t isNuMI: " << f.isNuMI();
  os << "\n\t Last Overall Timestamp: " << f.getLastTimestamp();
  os << "\n\t Last BNB Timestamp: " << f.getLastTimestampBNB();
  os << "\n\t Last NuMI Timestamp: " << f.getLastTimestampNuMI();
  os << "\n\t Delta Gates: " << f.getDeltaGates();
  os << "\n\t Delta Gates BNB: " << f.getDeltaGatesBNB();
  os << "\n\t Delta Gates NuMI: " << f.getDeltaGatesNuMI();
  
  os << std::endl;
  return os;
}
/*
std::ostream & icarus::operator << (std::ostream & os, ICARUSTriggerUDPFragmentMetadata const & m) {
  os << "\n\t Name: " << m.getName();
  os << "\n\t Event Number: " << m.getEventNo();
  os << "\n\t Seconds: " << m.getSeconds();
  os << "\n\t Nanoseconds: " << m.getNanoSeconds();
  os << "\n\t WR: " << m.getWRName();
  os << "\n\t WR Event Number: " << m.getWREventNo();
  os << "\n\t WR Seconds: " << m.getWRSeconds();
  os << "\n\t WR Nanoseconds: " << m.getWRNanoSeconds();
  os << std::endl;
  return os;
}
*/


bool icarus::ICARUSTriggerUDPFragment::Verify() const {
  return true;
}
