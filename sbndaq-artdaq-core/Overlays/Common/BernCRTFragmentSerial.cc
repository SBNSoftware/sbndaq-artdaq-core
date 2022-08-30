#include "sbndaq-artdaq-core/Overlays/Common/BernCRTFragmentSerial.hh"

std::ostream& sbndaq::operator<<(std::ostream& os, BernCRTFragmentSerial const& serial)
{
  FragmentSerialBase serial_base(serial);

  os << serial_base
     << "Data Payload Size: " << serial.data_payload_size
     << "Number of Hits:    " << serial.n_hits
     << serial.metadata;

  for(unsigned hit = 0; hit < serial.bern_crt_hits.size(); ++hit)
    os << "Hit: " << hit << '\n' << serial.bern_crt_hits.at(hit);

  return os;
}
