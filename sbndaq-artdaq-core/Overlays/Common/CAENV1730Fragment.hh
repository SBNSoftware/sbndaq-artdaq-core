#ifndef sbndaq_artdaq_core_Overlays_Common_CAENV1730Fragment_hh
#define sbndaq_artdaq_core_Overlays_Common_CAENV1730Fragment_hh

#include "artdaq-core/Data/Fragment.hh"
#include "cetlib_except/exception.h"

#define CAEN_V1730_MAX_CHANNELS 16

namespace sbndaq {
  
  class CAENV1730Fragment;

  struct CAENV1730FragmentMetadata;
  struct CAENV1730FragmentMetadataV0;
  struct CAENV1730EventHeader;
  struct CAENV1730Event;

}

struct sbndaq::CAENV1730EventHeader{

  uint32_t eventSize : 28;
  uint32_t marker    : 4;
  
  uint32_t channelMask_lo : 8;
  uint32_t pattern        : 16;
  uint32_t eventFormat    : 1;
  uint32_t reserved       : 1;
  uint32_t boardFail      : 1;
  uint32_t boardID        : 5;

  uint32_t eventCounter   : 24;
  uint32_t channelMask_hi : 8;

  uint32_t triggerTimeTag : 32;


  uint32_t ChannelMask() const { return (channelMask_hi << 8) + (channelMask_lo & 0xff) ; }

  // with ETTT disabled, trigger tag is 31+1 bits and pattern is used for LVDS info
  uint32_t triggerTime() const { return triggerTimeTag & 0x7fff'ffffU; }
  bool triggerTimeRollOver() const { return bool( triggerTimeTag & 0x8000'0000U ); }
  
  // with ETTT enabled, trigger tag 48 bits, pattern + triggerTimeTag; no LVDS info
  uint64_t extendedTriggerTime() const
    { return triggerTimeTag + (static_cast<uint64_t>(pattern) << 32U); }
  
};
static_assert(sizeof(sbndaq::CAENV1730EventHeader)==4*sizeof(uint32_t),"CAENV1730EventHeader not correct size.");


struct sbndaq::CAENV1730Event{
  CAENV1730EventHeader Header;
  uint16_t             DataBlock;
};

struct sbndaq::CAENV1730FragmentMetadata {

  static constexpr uint32_t markerWord = 0xdeadbeef; 
  uint32_t  version;  

  uint32_t  nChannels;
  uint32_t  nSamples;
  uint32_t  timeStampSec;
  uint32_t  timeStampNSec;
  uint32_t  postTriggerPercent;  

  uint32_t  chTemps[CAEN_V1730_MAX_CHANNELS];
  
  CAENV1730FragmentMetadata() : version(1) {}
  
  CAENV1730FragmentMetadata(uint32_t nc, uint32_t ns,
			    uint32_t ts_s, uint32_t ts_ns,
			    uint32_t ptp,
			    uint32_t chtemp[CAEN_V1730_MAX_CHANNELS]) :
    version(1),
    nChannels(nc),nSamples(ns),
    timeStampSec(ts_s),timeStampNSec(ts_ns),
    postTriggerPercent(ptp)
  {
    memcpy(chTemps,chtemp,CAEN_V1730_MAX_CHANNELS*sizeof(uint32_t));
  }

  size_t ExpectedDataSize() const 
  { return (sizeof(CAENV1730EventHeader) + 
	    nChannels * nSamples * sizeof(uint16_t)); }
};

struct sbndaq::CAENV1730FragmentMetadataV0 {

  uint32_t  nChannels;
  uint32_t  nSamples;
  uint32_t  timeStampSec;
  uint32_t  timeStampNSec;

  uint32_t  chTemps[CAEN_V1730_MAX_CHANNELS];
  
  CAENV1730FragmentMetadataV0() {}
  
  CAENV1730FragmentMetadataV0(uint32_t nc, uint32_t ns,
			    uint32_t ts_s, uint32_t ts_ns,
			    uint32_t chtemp[CAEN_V1730_MAX_CHANNELS]) :
    nChannels(nc),nSamples(ns),
    timeStampSec(ts_s),timeStampNSec(ts_ns)
  {
    memcpy(chTemps,chtemp,CAEN_V1730_MAX_CHANNELS*sizeof(uint32_t));
  }

  size_t ExpectedDataSize() const 
  { return (sizeof(CAENV1730EventHeader) + 
	    nChannels * nSamples * sizeof(uint16_t)); }
};

class sbndaq::CAENV1730Fragment{

public:
  
  CAENV1730Fragment(artdaq::Fragment const& f) : fFragment(f) {}

  CAENV1730FragmentMetadata const Metadata() const { 
    CAENV1730FragmentMetadata metadata = *(fFragment.metadata<sbndaq::CAENV1730FragmentMetadata>()); 
  
    if(metadata.markerWord != 0xdeadbeef){
      auto const* mv0_ptr         = fFragment.metadata<sbndaq::CAENV1730FragmentMetadataV0>();

      metadata = CAENV1730FragmentMetadata();
      metadata.version = 0;
      metadata.postTriggerPercent = 9999; //dummy number
  
      metadata.nChannels     = mv0_ptr->nChannels;
      metadata.nSamples      = mv0_ptr->nSamples;
      metadata.timeStampSec  = mv0_ptr->timeStampSec;
      metadata.timeStampNSec = mv0_ptr->timeStampNSec;
      
      memcpy(const_cast<uint32_t*>(mv0_ptr->chTemps), metadata.chTemps, sizeof(mv0_ptr->chTemps)*sizeof(uint32_t));
     
      return metadata;
    }
    else
    { return *(fFragment.metadata<sbndaq::CAENV1730FragmentMetadata>()); }
  }

  CAENV1730Event const* Event() const
  { return reinterpret_cast<CAENV1730Event const*>(fFragment.dataBeginBytes()); }

  size_t DataPayloadSize() const
  { return fFragment.dataSizeBytes(); }

  size_t ExpectedDataSize() const 
  { return Metadata().ExpectedDataSize(); }

  bool Verify() const;

private:
  artdaq::Fragment fFragment;


};

#endif /* sbndaq_datatypes_Overlays_CAENV1730Fragment_hh */
