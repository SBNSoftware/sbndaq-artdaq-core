#ifndef sbndaq_artdaq_core_Overlays_ICARUS_ICARUSTriggerUDPFragment_hh
#define sbndaq_artdaq_core_Overlays_ICARUS_ICARUSTriggerUDPFragment_hh

#include "artdaq-core/Data/Fragment.hh"
#include "cetlib_except/exception.h"

#include <iostream>
#include <chrono>


namespace icarus {

  struct ICARUSTriggerInfo;
  ICARUSTriggerInfo parse_ICARUSTriggerString(const char*);
  class ICARUSTriggerUDPFragment;
  std::ostream & operator << (std::ostream &, ICARUSTriggerUDPFragment const &);

  //struct ICARUSTriggerUDPFragmentMetadata;
  class ICARUSTriggerUDPFragmentMetadata;
  //std::ostream & operator << (std::ostream &, ICARUSTriggerUDPFragmentMetadata const &);
}

struct icarus::ICARUSTriggerInfo
{
  std::string name;
  long event_no;
  long seconds;
  long nanoseconds;
  std::string wr_name;
  long wr_event_no;
  long wr_seconds;
  long wr_nanoseconds;
  long gate_id;
  int gate_type;
  ICARUSTriggerInfo() {
    name = ""; 
    event_no = -1; 
    seconds = -2; 
    nanoseconds = -3; 
    wr_name = ""; 
    wr_event_no = -1; 
    wr_seconds = -2; 
    wr_nanoseconds = -3;
    gate_id = -4;
    gate_type = 0;
  }
  uint64_t getNanoseconds_since_UTC_epoch() {
    if(wr_seconds == -2 || wr_nanoseconds == -3)
      return 0;
    int correction = 0;
    if(wr_seconds >= 1483228800)
      correction = 37;
    return (wr_seconds-correction)*1e9 + wr_nanoseconds;
  }
  
};

icarus::ICARUSTriggerInfo icarus::parse_ICARUSTriggerString(const char* buffer)
{
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
  ICARUSTriggerInfo info;
  info.name = sections[0];
  //t->setHardwareTS_Type(sections[0]);                                                                                       
  info.event_no = std::stol(sections[1]);
  info.seconds = std::stoi(sections[2]);
  info.nanoseconds = std::stol(sections[3]);
  if(sections.size() > 5)
    {
      info.wr_name = sections[4];
      info.wr_event_no = std::stol(sections[5]);
      info.wr_seconds = std::stol(sections[6]);
      info.wr_nanoseconds = std::stol(sections[7]);
      info.gate_id = std::stol(sections[9]);
      info.gate_type = std::stoi(sections[11]);
    }
  return info;
}


//struct icarus::ICARUSTriggerUDPFragmentMetadata
class icarus::ICARUSTriggerUDPFragmentMetadata
{

public:  
  ICARUSTriggerUDPFragmentMetadata() {}
  ICARUSTriggerUDPFragmentMetadata(uint64_t ntp_t, uint64_t last_ts, long dg) : ntp_time(ntp_t), last_timestamp(last_ts), delta_gates(dg) {}
  
  uint64_t getNTPTime() const
  { return ntp_time; } 

  uint64_t getLastTimestamp() const
  { return last_timestamp; }
  
  long getDeltaGates() const
  { return delta_gates; }

private:
  uint64_t ntp_time;
  uint64_t last_timestamp;
  long delta_gates;

};

class icarus::ICARUSTriggerUDPFragment{

public:
  
  ICARUSTriggerUDPFragment(artdaq::Fragment const &f) : fFragment(f) {info = parse_ICARUSTriggerString((const char*)fFragment.dataBeginBytes());}
  ICARUSTriggerUDPFragmentMetadata const * Metadata() const
  { return fFragment.metadata<ICARUSTriggerUDPFragmentMetadata>(); }
  
  size_t DataPayloadSize() const
  { return fFragment.dataSizeBytes(); }

  std::string GetDataString() const
  { 
    std::string data((char*)fFragment.dataBeginBytes(), fFragment.dataSizeBytes());
    return data;
  }

  /*
  size_t ExpectedDataSize() const
  { return Metadata()->ExpectedDataSize(); }
  */
  
  std::string getName() const
  { return info.name; }

  long getEventNo() const
  { return info.event_no; }

  long getSeconds() const
  { return info.seconds; }

  long getNanoSeconds() const
  { return info.nanoseconds; }

  std::string getWRName() const
  { return info.wr_name; }

  long getWREventNo() const
  { return info.wr_event_no; }

  long getWRSeconds() const
  { return info.wr_seconds; }

  long getWRNanoSeconds() const
  { return info.wr_nanoseconds; }
  
  long getGateID() const
  { return info.gate_id; }

  int getGateType() const
  { return info.gate_type; }

  uint64_t getLastTimestamp() const
  { return Metadata()->getLastTimestamp(); }

  uint64_t getNTPTime() const
  { return Metadata()->getNTPTime(); } 

  long getDeltaGates() const
  { return Metadata()->getDeltaGates(); }

  bool Verify() const;
  
private:
  artdaq::Fragment const & fFragment;
  ICARUSTriggerInfo info;
};

#endif /* sbndaq_datatypes_Overlays_ICARUSTriggerUDPFragment_hh */
