#ifndef sbndaq_artdaq_core_Overlays_ICARUS_ICARUSTriggerUDPFragment_hh
#define sbndaq_artdaq_core_Overlays_ICARUS_ICARUSTriggerUDPFragment_hh

#include "artdaq-core/Data/Fragment.hh"
#include "cetlib_except/exception.h"

#include <iostream>

namespace icarus {

  class ICARUSTriggerUDPFragment;
  std::ostream & operator << (std::ostream &, ICARUSTriggerUDPFragment const &);

  //struct ICARUSTriggerUDPFragmentMetadata;
  class ICARUSTriggerUDPFragmentMetadata;
  std::ostream & operator << (std::ostream &, ICARUSTriggerUDPFragmentMetadata const &);
}

//struct icarus::ICARUSTriggerUDPFragmentMetadata
class icarus::ICARUSTriggerUDPFragmentMetadata
{

public:  
  ICARUSTriggerUDPFragmentMetadata() {}
  ICARUSTriggerUDPFragmentMetadata(int n, int ev, int sec, long nsec) : name(n), event_no(ev), seconds(sec), nanoseconds(nsec) {}

  int getName() const
  { return name; }
  
  int getEventNo() const
  { return event_no; }

  int getSeconds() const
  { return seconds; }

  long getNanoSeconds() const
  { return nanoseconds; }

private:
  int name;
  int event_no;
  int seconds;
  long nanoseconds;

};

class icarus::ICARUSTriggerUDPFragment{

public:
  
  ICARUSTriggerUDPFragment(artdaq::Fragment const &f) : fFragment(f) {}
  ICARUSTriggerUDPFragmentMetadata const * Metadata() const
  { return fFragment.metadata<ICARUSTriggerUDPFragmentMetadata>(); }
  
  size_t DataPayloadSize() const
  { return fFragment.dataSizeBytes(); }
  /*
  size_t ExpectedDataSize() const
  { return Metadata()->ExpectedDataSize(); }
  */

  int getName() const
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
