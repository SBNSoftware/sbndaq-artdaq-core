#ifndef sbndaq_artdaq_core_Overlays_ICARUS_ICARUSTriggerUDPFragment_hh
#define sbndaq_artdaq_core_Overlays_ICARUS_ICARUSTriggerUDPFragment_hh

#include "artdaq-core/Data/Fragment.hh"
#include "cetlib_except/exception.h"

namespace sbndaq {

  class ICARUSTriggerUDPFragment;

  struct ICARUSTriggerUDPFragmentMetadata;

}

struct sbndaq::ICARUSTriggerUDPFragmentMetadata
{
  std::string name;
  int event_no;
  int seconds;
  long nanoseconds;
  
  ICARUSTriggerUDPFragmentMetadata() {}
  ICARUSTriggerUDPFragmentMetadata(std::string n, int ev, int sec, long nsec) : name(n), event_no(ev), seconds(sec), nanoseconds(nsec) {}

  std::string getName() const
  { return name; }
  
  int getEventNo() const
  { return event_no; }

  int getSeconds() const
  { return seconds; }

  long getNanoSeconds() const
  { return nanoseconds; }
};

class sbndaq::ICARUSTriggerUDPFragment{

public:
  
  ICARUSTriggerUDPFragment(artdaq::Fragment const &f) : fFragment(f) {}
  ICARUSTriggerUDPFragmentMetadata const * Metadata() const
  { return fFragment.metadata<sbndaq::ICARUSTriggerUDPFragmentMetadata>(); }
  
  size_t DataPayloadSize() const
  { return fFragment.dataSizeBytes(); }
  /*
  size_t ExpectedDataSize() const
  { return Metadata()->ExpectedDataSize(); }
  */

  std::string getName() const
  { return Metadata()->getName(); }
  
  int getEventNo() const
  { return Metadata()->getEventNo(); }
  
  int getSeconds() const
  { return Metadata()->getSeconds(); }
  
  long getNanoSeconds() const
  { return Metadata()->getNanoSeconds(); }
  
  bool Verify() const;
  
private:
  artdaq::Fragment fFragment;

};

#endif /* sbndaq_datatypes_Overlays_ICARUSTriggerUDPFragment_hh */
