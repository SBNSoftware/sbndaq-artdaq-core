#include "sbndaq-artdaq-core/Overlays/Common/CAENV1740Fragment.hh"
#include "cetlib_except/exception.h"

bool sbndaq::CAENV1740Fragment::Verify() const {

  if(Metadata()->nChannels!=CAEN_V1740_MAX_CHANNELS) 
    return false;

  if(ExpectedDataSize() != DataPayloadSize())
    return false;

  return true;
}
