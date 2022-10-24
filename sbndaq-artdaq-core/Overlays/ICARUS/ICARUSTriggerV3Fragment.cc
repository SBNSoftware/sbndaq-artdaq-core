#include "sbndaq-artdaq-core/Overlays/ICARUS/ICARUSTriggerV3Fragment.hh"
#include "cetlib_except/exception.h"



std::ostream & icarus::operator << (std::ostream & os, ICARUSTriggerV3Fragment const & f) {
 
  os << "ICARUSTriggerV3Fragment: " << f.GetDataString();
  os << "\n\t Event Number: " << f.getWREventNo();
  os << "\n\t Seconds since UTC epoch: " << f.getWRSeconds();
  os << "\n\t Nanoseconds: " << f.getWRNanoSeconds();
  os << "\n\t Gate ID: " << f.getGateID();
  os << "\n\t Gate Type: " << f.getGateType();
  os << "\n\t isBNB: " << f.isBNB();
  os << "\n\t isNuMI: " << f.isNuMI();
  os << "\n\t Last Overall Timestamp: " << f.getLastTimestamp();
  os << "\n\t Last BNB Majority Timestamp: " << f.getLastTimestampBNBMaj();
  os << "\n\t Last NuMI Majority Timestamp: " << f.getLastTimestampNuMIMaj();
  os << "\n\t Delta Gates: " << f.getDeltaGates();
  os << "\n\t Delta Gates BNB Majority: " << f.getDeltaGatesBNBMaj();
  os << "\n\t Delta Gates NuMI Majority: " << f.getDeltaGatesNuMIMaj();
  
  os << std::endl;
  return os;
}
/*
std::ostream & icarus::operator << (std::ostream & os, ICARUSTriggerV3FragmentMetadata const & m) {
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


bool icarus::ICARUSTriggerV3Fragment::Verify() const {
  return true;
}
