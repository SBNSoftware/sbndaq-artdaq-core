#include "sbndaq-artdaq-core/Overlays/SBND/CAENV1730FragmentSerial.hh"

std::ostream& sbndaq::operator<<(std::ostream& os, CAENV1730FragmentSerial const& serial)
{
  FragmentSerialBase serial_base(serial);

  os << serial_base
     << serial.metadata << '\n'
     << serial.event << '\n';

  return os;
}
