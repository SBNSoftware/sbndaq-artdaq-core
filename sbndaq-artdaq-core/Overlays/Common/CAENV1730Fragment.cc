#include "sbndaq-artdaq-core/Overlays/Common/CAENV1730Fragment.hh"
#include "cetlib_except/exception.h"

std::ostream& sbndaq::operator<<(std::ostream& os, CAENV1730Fragment const& fragment)
{
  os << "CAENV1730Fragment:\n"
     << *fragment.Metadata() << '\n'
     << *fragment.Event() << '\n';

  return os;
}

std::ostream& sbndaq::operator<<(std::ostream& os, CAENV1730FragmentMetadata const& metadata)
{
  os << "CAENV1730FragmentMetadata:\n"
     << "\tnChannels: " << metadata.nChannels << '\n'
     << "\tnSamples: " << metadata.nSamples << '\n'
     << "\ttimeStampSec: " << metadata.timeStampSec << '\n'
     << "\ttimeStampNSec: " << metadata.timeStampNSec << '\n';
 
  for(unsigned i = 0; i < CAEN_V1730_MAX_CHANNELS; ++i)
    os << "\t\tChannel: " << i << " Temp: " << metadata.chTemps[i] << '\n';

  return os;
}

std::ostream& sbndaq::operator<<(std::ostream& os, CAENV1730EventHeader const& header)
{
  os << "CAENV1730EventHeader:\n"
     << "\teventSize: " << header.eventSize << '\n'
     << "\tmarker: " << header.marker << '\n'
     << "\tchannelMask_lo: " << header.channelMask_lo << '\n'
     << "\tpattern: " << header.pattern << '\n'
     << "\teventFormat: " << header.eventFormat << '\n'
     << "\treserved: " << header.reserved << '\n'
     << "\tboardFail: " << header.boardFail << '\n'
     << "\tboardID: " << header.boardID << '\n'
     << "\teventCounter: " << header.eventCounter << '\n'
     << "\tchannelMask_hi: " << header.channelMask_hi << '\n'
     << "\ttriggerTimeTag: " << header.triggerTimeTag << '\n';

  return os;
}

std::ostream& sbndaq::operator<<(std::ostream& os, CAENV1730Event const& event)
{
  os << "CAENV1730Event:\n"
     << "\tDataBlock: " << event.DataBlock << '\n'
     << event.Header;

  return os;
}

bool sbndaq::CAENV1730Fragment::Verify() const {

  if(Metadata()->nChannels!=CAEN_V1730_MAX_CHANNELS) 
    return false;

  if(ExpectedDataSize() != DataPayloadSize())
    return false;

  return true;
}
