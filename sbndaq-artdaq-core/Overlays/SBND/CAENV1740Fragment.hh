#ifndef sbndaq_artdaq_core_Overlays_Common_CAENV1740Fragment_hh
#define sbndaq_artdaq_core_Overlays_Common_CAENV1740Fragment_hh

#include "artdaq-core/Data/Fragment.hh"
#include "cetlib_except/exception.h"

#define CAEN_V1740_MAX_CHANNELS 64

namespace sbndaq {
  
  class CAENV1740Fragment;

  struct CAENV1740FragmentMetadata;
  struct CAENV1740EventHeader;
  struct CAENV1740Event;

}

struct sbndaq::CAENV1740EventHeader{

  uint32_t eventSize : 28;
  uint32_t marker    : 4;
  
  uint32_t groupMask : 8;
  uint32_t pattern        : 16;
  uint32_t eventFormat    : 1; // set to 0 for V1740
  uint32_t reserved       : 1;
  uint32_t boardFail      : 1;
  uint32_t boardID        : 5;

  uint32_t eventCounter   : 24;
  uint32_t reserved_hi : 8;

  uint32_t triggerTimeTag : 32;


  uint32_t GroupMask() const { return (groupMask & 0xff); }

  // with ETTT disabled, trigger tag is 31+1 bits and pattern is used for LVDS info
  uint32_t triggerTime() const { return (triggerTimeTag & 0x7fffffff); }
  bool triggerTimeRollOver() const { return bool( triggerTimeTag & 0x80000000 ); }
  
  // with ETTT enabled, trigger tag 48 bits, pattern + triggerTimeTag; no LVDS info
  uint64_t extendedTriggerTime() const
    { return triggerTimeTag + (static_cast<uint64_t>(pattern) << 32U); }
  
};
static_assert(sizeof(sbndaq::CAENV1740EventHeader)==4*sizeof(uint32_t),"CAENV1740EventHeader not correct size.");


struct sbndaq::CAENV1740Event{
  CAENV1740EventHeader Header;
  uint16_t             DataBlock;
};

struct sbndaq::CAENV1740FragmentMetadata {

  uint32_t  nChannels;
  uint32_t  nSamples;
  uint32_t  timeStampSec;
  uint32_t  timeStampNSec;

  // uint32_t  chTemps[CAEN_V1740_MAX_CHANNELS]; // jcrespo: not available for V1740
  
  CAENV1740FragmentMetadata() {}

  // jcrespo: channel temperature not available for V1740
//   CAENV1740FragmentMetadata(uint32_t nc, uint32_t ns,
// 			    uint32_t ts_s, uint32_t ts_ns,
// 			    uint32_t chtemp[CAEN_V1740_MAX_CHANNELS]) :
//     nChannels(nc),nSamples(ns),
//     timeStampSec(ts_s),timeStampNSec(ts_ns)
//   {
//     memcpy(chTemps,chtemp,CAEN_V1740_MAX_CHANNELS*sizeof(uint32_t));
//   }

//   size_t ExpectedDataSize() const
//   { return (sizeof(CAENV1740EventHeader) +
// 	    nChannels * nSamples * sizeof(uint16_t)); }
  
// };

  CAENV1740FragmentMetadata(uint32_t nc, uint32_t ns,
			    uint32_t ts_s, uint32_t ts_ns) :
    nChannels(nc),nSamples(ns),
    timeStampSec(ts_s),timeStampNSec(ts_ns)
  {
    // memcpy(chTemps,chtemp,CAEN_V1740_MAX_CHANNELS*sizeof(uint32_t));
  }

  size_t ExpectedDataSize() const 
  { return (sizeof(CAENV1740EventHeader) + 
	    nChannels * nSamples * sizeof(uint16_t)); }
  
};


class sbndaq::CAENV1740Fragment{

public:
  
  CAENV1740Fragment(artdaq::Fragment const& f) : fFragment(f) {}

  CAENV1740FragmentMetadata const* Metadata() const
  { return fFragment.metadata<sbndaq::CAENV1740FragmentMetadata>(); }

  CAENV1740Event const* Event() const
  { return reinterpret_cast<CAENV1740Event const*>(fFragment.dataBeginBytes()); }

  size_t DataPayloadSize() const
  { return fFragment.dataSizeBytes(); }

  size_t ExpectedDataSize() const 
  { return Metadata()->ExpectedDataSize(); }

  bool Verify() const;

private:
  artdaq::Fragment fFragment;


};

#endif /* sbndaq_datatypes_Overlays_CAENV1740Fragment_hh */
