#ifndef sbndaq_artdaq_core_Overlays_ICARUS_ICARUSTriggerUDPFragment_hh
#define sbndaq_artdaq_core_Overlays_ICARUS_ICARUSTriggerUDPFragment_hh

#include "artdaq-core/Data/Fragment.hh"
#include "sbndaq-artdaq-core/Trace/trace_defines.h"
#include "cetlib_except/exception.h"

#include <iostream>
#include <chrono>
#include <string>
#include <map>


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
  int version;
  std::string name;
  long event_no;
  long seconds;
  long nanoseconds;
  std::string wr_name;
  long wr_event_no;
  long wr_seconds;
  long wr_nanoseconds;
  int enable_type;
  long enable_seconds;
  long enable_nanoseconds;
  long gate_id;
  long gate_id_BNB;
  long gate_id_NuMI;
  long gate_id_BNBOff;
  long gate_id_NuMIOff;
  int gate_type;
  long beam_seconds;
  long beam_nanoseconds;
  int trigger_type;
  int trigger_source;
  std::string cryo1_e_conn_0;
  std::string cryo1_e_conn_2;
  std::string cryo2_w_conn_0;
  std::string cryo2_w_conn_2;
  long cryo1_east_counts;
  long cryo2_west_counts;

  ICARUSTriggerInfo() {
    version = 0;
    name = ""; 
    event_no = -1; 
    seconds = -2; 
    nanoseconds = -3; 
    wr_name = ""; 
    wr_event_no = -1; 
    wr_seconds = -2; 
    wr_nanoseconds = -3;
    enable_type = -1;
    enable_seconds = 0;
    enable_nanoseconds = 0;
    gate_id = -4;
    gate_type = 0;
    gate_id_BNB = -4;
    gate_id_NuMI = -4;
    gate_id_BNBOff = -4;
    gate_id_NuMIOff = -4;
    beam_seconds = 0;
    beam_nanoseconds = 0;
    trigger_type = 0;
    trigger_source = 0;
    cryo1_e_conn_0 = "";
    cryo1_e_conn_2 = "";
    cryo2_w_conn_0 = "";
    cryo2_w_conn_2 = "";
    cryo1_east_counts = -1;
    cryo2_west_counts = -1;
    
  }
  uint64_t getNanoseconds_since_UTC_epoch() {
    if(wr_seconds == -2 || wr_nanoseconds == -3)
      return 0;
    int correction = 0;
    if(wr_seconds >= 1483228800)
      correction = 37;
    uint64_t const corrected_ts
      { (wr_seconds-correction)*1'000'000'000ULL + wr_nanoseconds };
    return corrected_ts;
  }
  
};

icarus::ICARUSTriggerInfo icarus::parse_ICARUSTriggerString(const char* buffer)
{
  std::string data_input = buffer;
  size_t pos = 0;
  std::string delimiter = ",";
  std::vector<std::string> sections;
  std::string token = "";
  std::map<std::string, std::string> elements;
  std::string beam_ts = "Beam_TS";
  std::string wr_ts = "WR_TS";
  std::string local_ts = "Local_TS";
  std::string enable_ts = "Enable_TS";
  
  while ((pos = data_input.find(delimiter)) != std::string::npos) {
    token = data_input.substr(0, pos);
    sections.push_back(token);
    data_input.erase(0, pos + delimiter.length());
  }
  sections.push_back(data_input);
  //std::string trig_name = sections[0];                                                                                      
  ICARUSTriggerInfo info;
  //this should be agnostic to any length (and order) string that follows the key, value, format unless another timestamp is added
  /*
  for(unsigned int i = 0; i < sections.size(); i = i + 2)
  {
    //large or statement should do this just fine, need to separate out TS stuff
    //string.contains is not accessible with the gcc versions we use for this...
    if(sections[i].find(local_ts) != std::string::npos || sections[i].find(wr_ts) != std::string::npos || sections[i].find(beam_ts) != std::string::npos || sections[i].find(enable_ts) != std::string::npos) 
    {
      //event number
      elements.insert(std::pair<std::string, std::string>(sections[i], sections[i+1]));
      std::string seco = "sec_" + sections[i];
      //seconds 
      elements.insert(std::pair<std::string, std::string>(seco, sections[i+2]));
      std::string nanoseco = "nanosec_" + sections[i];
      //nanoseconds
      elements.insert(std::pair<std::string, std::string>(nanoseco, sections[i+3]));
      i += 2;
    }
    else
      elements.insert(std::pair<std::string, std::string>(sections[i], sections[i+1]));
  }
  //read back map and store values
  std::map<std::string,std::string>::iterator it = elements.begin();
  for (it = elements.begin(); it != elements.end(); ++it)
  {
    //std::cout << it->first << " => " << it->second << '\n';
    //lots of if statements based on what's in there now, should stay agnostic if something isn't stored just set to a dummy value that is known, write up defaults here
    //string.contains not accessible with gcc versions we currently use, implement a workaround to find string within string 
    if(it->first.find("Version") != std::string::npos)
      info.version = std::stoi(it->second);
    if(it->first.find("Local_TS") != std::string::npos)
    {
      info.name = it->first;
      info.event_no = std::stol(it->second);
    }
    if(it->first.find("sec_Local") != std::string::npos)
      info.seconds = std::stol(it->second);
    if(it->first.find("nanosec_Local") != std::string::npos)
      info.nanoseconds = std::stol(it->second);
    if(it->first.find("WR_TS") != std::string::npos)
    {
      info.wr_name = it->first;
      info.wr_event_no = std::stol(it->second);
    }
    if(it->first.find("sec_WR") != std::string::npos)
      info.wr_seconds = std::stol(it->second);
    if(it->first.find("nanosec_WR") != std::string::npos)
      info.wr_nanoseconds = std::stol(it->second);
    if(it->first.find("Enable_Type") != std::string::npos)
      info.enable_type = std::stoi(it->second);
    if(it->first.find("sec_Enable") != std::string::npos)
      info.enable_seconds = std::stol(it->second);
    if(it->first.find("nanosec_Enable") != std::string::npos)
      info.enable_nanoseconds = std::stol(it->second);
    if(it->first.find("Gate ID") != std::string::npos && it->first.find("Beam") == std::string::npos && it->first.find("Offbeam") == std::string::npos ) //only store overall gate ID
      info.gate_id = std::stol(it->second);
    if(it->first.find("BNB Beam Gate ID") != std::string::npos)
       info.gate_id_BNB = std::stol(it->second);
    if(it->first.find("NuMI Beam Gate ID") != std::string::npos)
      info.gate_id_NuMI = std::stol(it->second);
    if(it->first.find("Offbeam BNB Beam Gate ID") != std::string::npos)
      info.gate_id_BNBOff = std::stol(it->second);
    if(it->first.find("Offbeam NuMI Beam Gate ID") != std::string::npos)
      info.gate_id_NuMIOff = std::stol(it->second);
    if(it->first.find("Gate Type") != std::string::npos)
      info.gate_type = std::stoi(it->second);
    if(it->first.find("sec_Beam") != std::string::npos)
      info.beam_seconds = std::stol(it->second);
    if(it->first.find("nanosec_Beam") != std::string::npos)
      info.beam_nanoseconds = std::stol(it->second);
    if(it->first.find("Trigger Type") != std::string::npos)
      info.trigger_type = std::stoi(it->second);
    if(it->first.find("Trigger Source") != std::string::npos)
      info.trigger_source = std::stoi(it->second);
   
    //leave these as strings, figure out how to decode them correctly in decoder program. Do they go into binary or a 64-bit unsigned int directly? (I'd think binary for 1 or 0 if given PMT fired)  
    if(it->first.find("Cryo1 EAST Connector 0") != std::string::npos)
      info.cryo1_e_conn_0 = it->second;
    if(it->first.find("Cryo1 EAST Connector 2") != std::string::npos)
      info.cryo1_e_conn_2 = it->second;
    if(it->first.find("Cryo2 WEST Connector 0") != std::string::npos)
      info.cryo2_w_conn_0 = it->second;
    if(it->first.find("Cryo2 WEST Connector 2") != std::string::npos)
      info.cryo2_w_conn_2 = it->second;

    //end PMT hex strings
    if(it->first.find("Cryo1 EAST Counts") != std::string::npos)
      info.cryo1_east_counts = std::stol(it->second);
    if(it->first.find("Cryo2 WEST Counts") != std::string::npos)
      info.cryo2_west_counts = std::stol(it->second);
      
  }
  */
  //old implementation preserved in case of emergency, rigid hardcoded version where string must appear in a specific order
  
  info.name = sections[2];
  //t->setHardwareTS_Type(sections[0]);                                                                                       
  info.event_no = std::stol(sections[3]);
  info.seconds = std::stoi(sections[4]);
  info.nanoseconds = std::stol(sections[5]);
  if(sections.size() > 5)
    {
      info.wr_name = sections[6];
      info.wr_event_no = std::stol(sections[7]);
      info.wr_seconds = std::stol(sections[8]);
      info.wr_nanoseconds = std::stol(sections[9]);
      info.gate_id = std::stol(sections[17]);
      info.gate_id_BNB = std::stol(sections[19]);
      info.gate_id_NuMI = std::stol(sections[21]);
      info.gate_id_BNBOff = std::stol(sections[23]);
      info.gate_id_NuMIOff = std::stol(sections[25]);
      info.gate_type = std::stoi(sections[27]);
      info.beam_seconds = std::stol(sections[30]);
      info.beam_nanoseconds = std::stol(sections[31]); 
 
    }
 
  return info;
}


//struct icarus::ICARUSTriggerUDPFragmentMetadata
class icarus::ICARUSTriggerUDPFragmentMetadata
{

public:  
  ICARUSTriggerUDPFragmentMetadata() {}
  ICARUSTriggerUDPFragmentMetadata(uint64_t ntp_t, 
				   uint64_t last_ts, 
				   uint64_t last_ts_bnb, uint64_t last_ts_numi, uint64_t last_ts_bnboff,
				   uint64_t last_ts_numioff,uint64_t last_ts_other,
				   long dg,
				   long dg_bnb, long dg_numi, long dg_bnboff, long dg_numioff,  long dg_other) 
    : ntp_time(ntp_t)
    , last_timestamp(last_ts)
    , last_timestamp_bnb(last_ts_bnb)
    , last_timestamp_numi(last_ts_numi)
    , last_timestamp_bnboff(last_ts_bnboff)
    , last_timestamp_numioff(last_ts_numioff)  
    , last_timestamp_other(last_ts_other)
    , delta_gates(dg) 
    , delta_gates_bnb(dg_bnb) 
    , delta_gates_numi(dg_numi)
    , delta_gates_bnboff(dg_bnboff)
    , delta_gates_numioff(dg_numioff)
    , delta_gates_other(dg_other) 
  {}
  
  uint64_t getNTPTime() const
  { return ntp_time; } 

  uint64_t getLastTimestamp() const
  { return last_timestamp; }

  uint64_t getLastTimestampBNB() const
  { return last_timestamp_bnb; }
  
  uint64_t getLastTimestampNuMI() const
  { return last_timestamp_numi; }

  uint64_t getLastTimestampBNBOff() const
  { return last_timestamp_bnboff; }
  
  uint getLastTimestampNuMIOff() const
  { return last_timestamp_numioff; }
  
  uint64_t getLastTimestampOther() const
  { return last_timestamp_other; }

  long getDeltaGates() const
  { return delta_gates; }

  long getDeltaGatesBNB() const
  { return delta_gates_bnb; }

  long getDeltaGatesNuMI() const
  { return delta_gates_numi; }

  long getDeltaGatesBNBOff() const
  { return delta_gates_bnboff; }
  
  long getDeltaGatesNuMIOff() const
  { return delta_gates_numioff; }

  long getDeltaGatesOther() const
  { return delta_gates_other; }

private:
  uint64_t ntp_time;
  uint64_t last_timestamp;
  uint64_t last_timestamp_bnb;
  uint64_t last_timestamp_numi;
  uint64_t last_timestamp_bnboff;
  uint64_t last_timestamp_numioff;
  uint64_t last_timestamp_other;

  long delta_gates;
  long delta_gates_bnb;
  long delta_gates_numi;
  long delta_gates_bnboff;
  long delta_gates_numioff;
  long delta_gates_other;


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

  long getGateIDBNB() const
  { return info.gate_id_BNB; }
  
  long getGateIDNuMI() const
  { return info.gate_id_NuMI; }
  
  long getGateIDBNBOff() const
  { return info.gate_id_BNBOff; }
  
  long gateGateIDNuMIOff() const
  { return info.gate_id_NuMIOff; }

  bool isBNB() const
  { return getGateType()==1; }

  bool isNuMI() const
  { return getGateType()==2; }

  bool isBNBOff() const
  { return getGateType()==3; }
  
  bool isNuMIOff() const
  { return getGateType()==4; }

  int getGateType() const
  { return info.gate_type; }
  
  long getBeamSeconds() const
  { return info.beam_seconds; }

  long getBeamNanoSeconds() const
  { return info.beam_nanoseconds; }
  
  uint64_t getLastTimestamp() const
  { return Metadata()->getLastTimestamp(); }

  uint64_t getNTPTime() const
  { return Metadata()->getNTPTime(); } 

  long getDeltaGates() const
  { return Metadata()->getDeltaGates(); }

  uint64_t getLastTimestampBNB() const
  { return Metadata()->getLastTimestampBNB(); }
  uint64_t getLastTimestampNuMI() const
  { return Metadata()->getLastTimestampNuMI(); }
  uint64_t getLastTimestampBNBOff() const
  { return Metadata()->getLastTimestampBNBOff(); }
  uint64_t getLastTimestampNuMIOff() const
  { return Metadata()->getLastTimestampNuMIOff(); }
  uint64_t getLastTimestampOther() const
  { return Metadata()->getLastTimestampOther(); }
  
  long getDeltaGatesBNB() const
  { return Metadata()->getDeltaGatesBNB(); }
  long getDeltaGatesNuMI() const
  { return Metadata()->getDeltaGatesNuMI(); }
  long getDeltaGatesBNBOff() const
  { return Metadata()->getDeltaGatesBNBOff(); }
  long getDeltaGatesNuMIOff() const
  { return Metadata()->getDeltaGatesNuMIOff(); }
  long getDeltaGatesOther() const
  { return Metadata()->getDeltaGatesOther(); }

  bool Verify() const;
  
private:
  artdaq::Fragment const & fFragment;
  ICARUSTriggerInfo info;
};

#endif /* sbndaq_datatypes_Overlays_ICARUSTriggerUDPFragment_hh */
