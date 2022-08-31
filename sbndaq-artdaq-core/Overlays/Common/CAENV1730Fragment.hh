#ifndef sbndaq_artdaq_core_Overlays_Common_CAENV1730Fragment_hh
#define sbndaq_artdaq_core_Overlays_Common_CAENV1730Fragment_hh

#include "artdaq-core/Data/Fragment.hh"
#include "cetlib_except/exception.h"

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/split_member.hpp>

#define CAEN_V1730_MAX_CHANNELS 16

namespace sbndaq {
  
  class CAENV1730Fragment;
  std::ostream& operator<<(std::ostream&, CAENV1730Fragment const&);

  struct CAENV1730FragmentMetadata;
  std::ostream& operator<<(std::ostream&, CAENV1730FragmentMetadata const&);
  struct CAENV1730EventHeader;
  std::ostream& operator<<(std::ostream&, CAENV1730EventHeader const&);
  struct CAENV1730Event;
  std::ostream& operator<<(std::ostream&, CAENV1730Event const&);
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
  
private:
  friend class boost::serialization::access;
  template<class Archive>
  void save(Archive & ar, const unsigned int /*version*/) const
  {
    uint32_t serial_eventSize = eventSize;
    uint32_t serial_marker = marker;
    uint32_t serial_channelMask_lo = channelMask_lo;
    uint32_t serial_pattern = pattern;
    uint32_t serial_eventFormat = eventFormat;
    uint32_t serial_reserved = reserved;
    uint32_t serial_boardFail = boardFail;
    uint32_t serial_boardID = boardID;
    uint32_t serial_eventCounter = eventCounter;
    uint32_t serial_channelMask_hi = channelMask_hi;
    uint32_t serial_triggerTimeTag = triggerTimeTag;

    ar & serial_eventSize;
    ar & serial_marker;
    ar & serial_channelMask_lo;
    ar & serial_pattern;
    ar & serial_eventFormat;
    ar & serial_reserved;
    ar & serial_boardFail;
    ar & serial_boardID;
    ar & serial_eventCounter;
    ar & serial_channelMask_hi;
    ar & serial_triggerTimeTag;
  }

  template<class Archive>
  void load(Archive & ar, const unsigned int /*version*/)
  {
    uint32_t serial_eventSize, serial_marker, serial_channelMask_lo,
      serial_pattern, serial_eventFormat, serial_reserved, serial_boardFail,
      serial_boardID, serial_eventCounter, serial_channelMask_hi, serial_triggerTimeTag;

    ar & serial_eventSize;
    ar & serial_marker;
    ar & serial_channelMask_lo;
    ar & serial_pattern;
    ar & serial_eventFormat;
    ar & serial_reserved;
    ar & serial_boardFail;
    ar & serial_boardID;
    ar & serial_eventCounter;
    ar & serial_channelMask_hi;
    ar & serial_triggerTimeTag;

    eventSize = serial_eventSize;
    marker = serial_marker;
    channelMask_lo = serial_channelMask_lo;
    pattern = serial_pattern;
    eventFormat = serial_eventFormat;
    reserved = serial_reserved;
    boardFail = serial_boardFail;
    boardID = serial_boardID;
    eventCounter = serial_eventCounter;
    channelMask_hi = serial_channelMask_hi;
    triggerTimeTag = serial_triggerTimeTag;
  }
  BOOST_SERIALIZATION_SPLIT_MEMBER()
};
static_assert(sizeof(sbndaq::CAENV1730EventHeader)==4*sizeof(uint32_t),"CAENV1730EventHeader not correct size.");

struct sbndaq::CAENV1730Event{
  CAENV1730EventHeader Header;
  uint16_t             DataBlock;

private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int /*version*/)
  {
    ar & Header;
    ar & DataBlock;
  }
};

struct sbndaq::CAENV1730FragmentMetadata {

  uint32_t  nChannels;
  uint32_t  nSamples;
  uint32_t  timeStampSec;
  uint32_t  timeStampNSec;

  uint32_t  chTemps[CAEN_V1730_MAX_CHANNELS];
  
  CAENV1730FragmentMetadata() {}
  
  CAENV1730FragmentMetadata(uint32_t nc, uint32_t ns,
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

private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int /*version*/)
  {
    ar & nChannels;
    ar & nSamples;
    ar & timeStampSec;
    ar & timeStampNSec;
    ar & chTemps;
  }
};

class sbndaq::CAENV1730Fragment{

public:
  
  CAENV1730Fragment(artdaq::Fragment const& f) : fFragment(f) {}

  CAENV1730FragmentMetadata const* Metadata() const
  { return fFragment.metadata<sbndaq::CAENV1730FragmentMetadata>(); }

  CAENV1730Event const* Event() const
  { return reinterpret_cast<CAENV1730Event const*>(fFragment.dataBeginBytes()); }

  size_t DataPayloadSize() const
  { return fFragment.dataSizeBytes(); }

  size_t ExpectedDataSize() const 
  { return Metadata()->ExpectedDataSize(); }

  bool Verify() const;

private:
  artdaq::Fragment fFragment;
};

#endif /* sbndaq_datatypes_Overlays_CAENV1730Fragment_hh */
