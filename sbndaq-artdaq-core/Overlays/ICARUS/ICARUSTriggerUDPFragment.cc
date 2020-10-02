#include "sbndaq-artdaq-core/Overlays/ICARUS/ICARUSTriggerUDPFragment.hh"
#include "cetlib_except/exception.h"



std::ostream & icarus::operator << (std::ostream & os, ICARUSTriggerUDPFragment const & f) {
  os << "ICARUSTriggerUDPFragment: " << *(f.Metadata());
  os << std::endl;
  return os;
}

std::ostream & icarus::operator << (std::ostream & os, ICARUSTriggerUDPFragmentMetadata const & m) {
  os << "\n\t Name: " << m.getName();
  os << "\n\t Event Number: " << m.getEventNo();
  os << "\n\t Seconds: " << m.getSeconds();
  os << "\n\t Nanoseconds: " << m.getNanoSeconds();
  os << std::endl;
  return os;
}



bool icarus::ICARUSTriggerUDPFragment::Verify() const {
  return true;
}
