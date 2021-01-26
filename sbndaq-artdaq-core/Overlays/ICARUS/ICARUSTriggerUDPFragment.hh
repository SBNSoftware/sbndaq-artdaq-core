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
  ICARUSTriggerUDPFragmentMetadata(int n, int ev, int sec, long nsec,int wr_n, int wr_ev, long wr_sec, long wr_nsec) : name(n), event_no(ev), seconds(sec), nanoseconds(nsec), wr_name(wr_n), wr_event_no(wr_ev), wr_seconds(wr_sec), wr_nanoseconds(wr_nsec) {}

  int getName() const
  { return name; }
  
  int getEventNo() const
  { return event_no; }

  int getSeconds() const
  { return seconds; }

  long getNanoSeconds() const
  { return nanoseconds; }

  int getWRName() const
  { return wr_name; }

  int getWREventNo() const
  { return wr_event_no; }

  long getWRSeconds() const
  { return wr_seconds; }

  long getWRNanoSeconds() const
  { return wr_nanoseconds; }

private:
  int name;
  int event_no;
  int seconds;
  long nanoseconds;
  int wr_name;
  int wr_event_no;
  long wr_seconds;
  long wr_nanoseconds;

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

  int getWRName() const
  { return Metadata()->getWRName(); }

  int getWREventNo() const
  { return Metadata()->getWREventNo(); }

  long getWRSeconds() const
  { return Metadata()->getWRSeconds(); }

  long getWRNanoSeconds() const
  { return Metadata()->getWRNanoSeconds(); }
  
  bool Verify() const;
  
private:
  artdaq::Fragment fFragment;

};

#endif /* sbndaq_datatypes_Overlays_ICARUSTriggerUDPFragment_hh */
