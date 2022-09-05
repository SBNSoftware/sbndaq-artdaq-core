#include "sbndaq-artdaq-core/Overlays/SBND/BernCRTFragmentV2Serial.hh"

std::ostream& sbndaq::operator<<(std::ostream& os, BernCRTFragmentV2Serial const& serial)
{
  FragmentSerialBase serial_base(serial);

  os << serial_base
     << "Data Payload Size: " << serial.data_payload_size << '\n'
     << "Number of Hits:    " << serial.n_hits << '\n'
     << serial.metadata << '\n';

  for(unsigned hit = 0; hit < serial.bern_crt_hits.size(); ++hit)
    os << "Hit: " << hit << '\n' << serial.bern_crt_hits.at(hit);

  return os;
}
