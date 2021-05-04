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
  ICARUSTriggerUDPFragmentMetadata(uint64_t ntp_t, uint64_t last_ts) : ntp_time(ntp_t), last_timestamp(last_ts) {}
  
  uint64_t getNTPTime() const
  { return ntp_time; } 

  uint64_t getLastTimestamp() const
  { return last_timestamp; }

private:
  uint64_t ntp_time;
  uint64_t last_timestamp;

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
  /*
  void setHardwareTS_Type(std::string n) const {name = n;} 
  void setHardwareEventNumber(long ev) const {event_no = ev;}
  void setHardwareSec(int s) const {seconds = s;}
  void setHardwareNanoSec(int ns) const {nanoseconds = ns;}
  void setTS_Type(std::string wr_n) const {wr_name = wr_n;}
  void setEventNumber(long wr_ev) const {wr_event_no = wr_ev;}
  void setEventTriggerSec(long wr_s) const {wr_seconds = wr_s;}
  void setEventTriggerNanoSec(long wr_ns) const {wr_nanoseconds = wr_ns;}
  */
  std::string getName() const
  { return name; }

  long getEventNo() const
  { return event_no; }

  int getSeconds() const
  { return seconds; }

  long getNanoSeconds() const
  { return nanoseconds; }

  std::string getWRName() const
  { return wr_name; }

  long getWREventNo() const
  { return wr_event_no; }

  long getWRSeconds() const
  { return wr_seconds; }

  long getWRNanoSeconds() const
  { return wr_nanoseconds; }

  uint64_t getLastTimestamp() const
  { return Metadata()->getLastTimestamp(); }

  //function to parse string
  int parse() const
  {
    char *buffer = fFragment.dataBeginBytes();
    std::string data_input = buffer;
    size_t pos = 0;
    std::string delimiter = ",";
    std::vector<std::string> sections;
    std::string token = "";
    while ((pos = data_input.find(delimiter)) != std::string::npos) {
      token = data_input.substr(0, pos);
      sections.push_back(token);
      data_input.erase(0, pos + delimiter.length());
    }
    sections.push_back(data_input);
    //std::string trig_name = sections[0];
    /*
    setHardwareTS_Type(trig_name);
    setHardwareEventNumber(std::stol(sections[1]));
    setHardwareSec(std::stoi(sections[2]));
    setHardwareNanoSec(std::stol(sections[3]));
    */
    name = sections[0];
    event_no = std::stol(sections[1]);
    seconds = std::stoi(sections[2]);
    nanoseconds = std::stol(sections[3]);
    if(sections.size() > 5)
    {
      wr_name = sections[4];
      wr_event_no = std::stol(sections[5]);
      wr_seconds = std::stol(sections[6]);
      wr_nanoseconds = std::stol(sections[7]);
      /*
      setTS_Type(wr_name);
      setEventNumber(std::stol(sections[5]));
      setEventTriggerSec(std::stol(sections[6]));
      setEventTriggerNanoSec(std::stol(sections[7]));
      */
    }

    return 1;
  }
  
  bool Verify() const;
  
private:
  artdaq::Fragment const & fFragment;
  std::string name;
  long event_no;
  uint64_t seconds;
  long nanoseconds;
  std::string wr_name;
  long wr_event_no;
  long wr_seconds;
  long wr_nanoseconds;

};

#endif /* sbndaq_datatypes_Overlays_ICARUSTriggerUDPFragment_hh */
