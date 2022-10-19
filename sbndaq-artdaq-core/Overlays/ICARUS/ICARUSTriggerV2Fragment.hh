#ifndef sbndaq_artdaq_core_Overlays_ICARUS_ICARUSTriggerV2Fragment_hh
#define sbndaq_artdaq_core_Overlays_ICARUS_ICARUSTriggerV2Fragment_hh

#include "sbndaq-artdaq-core/Overlays/ICARUS/ICARUSTriggerInfo.hh"
#include "artdaq-core/Data/Fragment.hh"
#include "sbndaq-artdaq-core/Trace/trace_defines.h"
#include "cetlib_except/exception.h"

#include <iostream>
#include <chrono>
#include <string>
#include <map>


namespace icarus {

  struct ICARUSTriggerInfo;
  ICARUSTriggerInfo parse_ICARUSTriggerV2String(const char*);
  class ICARUSTriggerV2Fragment;
  std::ostream & operator << (std::ostream &, ICARUSTriggerV2Fragment const &);

  //struct ICARUSTriggerV2FragmentMetadata;
  class ICARUSTriggerV2FragmentMetadata;
  //std::ostream & operator << (std::ostream &, ICARUSTriggerV2FragmentMetadata const &);
}

inline icarus::ICARUSTriggerInfo icarus::parse_ICARUSTriggerV2String(const char* buffer)
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
    TLOG(TLVL_INFO) << "Current section key string is: " << sections[i] << " and current section string value is: " << sections[i+1];
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
    TLOG(TLVL_INFO) << "Current element key is: " << it->first << " and current element value is: " << it->second;
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
  elements.clear();
  */
  //old implementation preserved in case of emergency, rigid hardcoded version where string must appear in a specific order
 
  info.version = std::stoi(sections[1]);
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
      info.enable_seconds = std::stol(sections[14]);
      info.enable_nanoseconds = std::stol(sections[15]);
      info.gate_id = std::stol(sections[17]);
      info.gate_id_BNB = std::stol(sections[19]);
      info.gate_id_NuMI = std::stol(sections[21]);
      info.gate_id_BNBOff = std::stol(sections[23]);
      info.gate_id_NuMIOff = std::stol(sections[25]);
      info.gate_type = std::stoi(sections[27]);
      info.beam_seconds = std::stol(sections[30]);
      info.beam_nanoseconds = std::stol(sections[31]); 
      info.trigger_type = std::stoi(sections[33]);
      info.trigger_source = std::stoi(sections[35]);
      //info.cryo1_east_counts = std::stol(sections[45]);
      //info.cryo2_west_counts = std::stol(sections[47]);
 
    }
 
  return info;
}


//struct icarus::ICARUSTriggerV2FragmentMetadata
class icarus::ICARUSTriggerV2FragmentMetadata
{

public:  
  ICARUSTriggerV2FragmentMetadata() {}
  ICARUSTriggerV2FragmentMetadata(uint64_t ntp_t, 
				   uint64_t last_ts, 
				   uint64_t last_ts_bnb, uint64_t last_ts_numi, uint64_t last_ts_bnboff,
				   uint64_t last_ts_numioff,uint64_t last_ts_calib, uint64_t last_ts_other,
				   int last_trig_type, long last_trig_bnb, long last_trig_numi, long last_trig_bnboff,
				   long last_trig_numioff, long last_trig_calib,
				   long total_trig_bnb, long total_trig_numi, long total_trig_bnboff,
				   long total_trig_numioff, long total_trig_calib,
				   long dg,
				   long dg_bnb, long dg_numi, long dg_bnboff, long dg_numioff, long dg_calib, long dg_other) 
    : ntp_time(ntp_t)
    , last_timestamp(last_ts)
    , last_timestamp_bnb(last_ts_bnb)
    , last_timestamp_numi(last_ts_numi)
    , last_timestamp_bnboff(last_ts_bnboff)
    , last_timestamp_numioff(last_ts_numioff)  
    , last_timestamp_calib(last_ts_calib)
    , last_timestamp_other(last_ts_other)
    , last_trigger_type(last_trig_type)
    , last_trigger_bnb(last_trig_bnb)
    , last_trigger_numi(last_trig_numi)
    , last_trigger_bnboff(last_trig_bnboff)
    , last_trigger_numioff(last_trig_numioff)
    , last_trigger_calib(last_trig_calib)
    , total_trigger_bnb(total_trig_bnb)
    , total_trigger_numi(total_trig_numi)
    , total_trigger_bnboff(total_trig_bnboff)
    , total_trigger_numioff(total_trig_numioff)
    , total_trigger_calib(total_trig_calib)
    , delta_gates(dg) 
    , delta_gates_bnb(dg_bnb) 
    , delta_gates_numi(dg_numi)
    , delta_gates_bnboff(dg_bnboff)
    , delta_gates_numioff(dg_numioff)
    , delta_gates_calib(dg_calib)
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
  
  uint64_t getLastTimestampNuMIOff() const
  { return last_timestamp_numioff; }

  uint64_t getLastTimestampCalib() const
  { return last_timestamp_calib; }
    
  uint64_t getLastTimestampOther() const
  { return last_timestamp_other; }

  int getLastTriggerType() const
  { return last_trigger_type; }

  long getLastTriggerBNB() const
  { return last_trigger_bnb; }
  
  long getLastTriggerNuMI() const
  { return last_trigger_numi; } 
  
  long getLastTriggerBNBOff() const
  { return last_trigger_bnboff; }

  long getLastTriggerNuMIOff() const
  { return last_trigger_numioff; }

  long getLastTriggerCalib() const
  { return last_trigger_calib; }

  long getTotalTriggerBNB() const
  { return total_trigger_bnb; }

  long getTotalTriggerNuMI() const
  { return total_trigger_numi; }
  
  long getTotalTriggerBNBOff() const
  { return total_trigger_bnboff; }

  long getTotalTriggerNuMIOff() const
  { return total_trigger_numioff; }

  long getTotalTriggerCalib() const
  { return total_trigger_calib; }

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

  long getDeltaGatesCalib() const
  { return delta_gates_calib; }

  long getDeltaGatesOther() const
  { return delta_gates_other; }

private:
  uint64_t ntp_time;
  uint64_t last_timestamp;
  uint64_t last_timestamp_bnb;
  uint64_t last_timestamp_numi;
  uint64_t last_timestamp_bnboff;
  uint64_t last_timestamp_numioff;
  uint64_t last_timestamp_calib;
  uint64_t last_timestamp_other;

  int last_trigger_type;
  long last_trigger_bnb;
  long last_trigger_numi;
  long last_trigger_bnboff;
  long last_trigger_numioff;
  long last_trigger_calib;
  long total_trigger_bnb;
  long total_trigger_numi;
  long total_trigger_bnboff;
  long total_trigger_numioff;
  long total_trigger_calib;

  long delta_gates;
  long delta_gates_bnb;
  long delta_gates_numi;
  long delta_gates_bnboff;
  long delta_gates_numioff;
  long delta_gates_calib;
  long delta_gates_other;


};

class icarus::ICARUSTriggerV2Fragment{

public:
  
  ICARUSTriggerV2Fragment(artdaq::Fragment const &f) : fFragment(f) {info = parse_ICARUSTriggerV2String((const char*)fFragment.dataBeginBytes());}
  ICARUSTriggerV2FragmentMetadata const * Metadata() const
  { return fFragment.metadata<ICARUSTriggerV2FragmentMetadata>(); }
  
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

  int getVersion() const
  { return info.version; }
  
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

  long getEnableSeconds() const
  { return info.enable_seconds; }
  
  long getEnableNanoSeconds() const
  { return info.enable_nanoseconds; }
  
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

  long getBeamSeconds() const
  { return info.beam_seconds; } 
  
  long getBeamNanoSeconds() const
  { return info.beam_nanoseconds; } 

  int getTriggerType() const
  { return info.trigger_type; }

  bool isMajority() const
  { return getTriggerType()==0; }

  bool isMinBias() const
  { return getTriggerType()==1; }

  int getTriggerSource() const
  { return info.trigger_source; }
  
  bool isEastTrigger() const
  { return getTriggerSource()==1; }
  
  bool isWestTrigger() const
  { return getTriggerSource()==2; }
  
  bool isBothTrigger() const
  { return getTriggerSource()==7; }
  
  bool isUnknownTrigger() const
  { return getTriggerSource()==0; }

  bool isBNB() const
  { return getGateType()==1; }

  bool isNuMI() const
  { return getGateType()==2; }

  bool isBNBOff() const
  { return getGateType()==3; }
  
  bool isNuMIOff() const
  { return getGateType()==4; }

  bool isCalibration() const
  { return getGateType()==5; }

  int getGateType() const
  { return info.gate_type; }
  /*
  long getCryoEastCounts() const
  { return info.cryo1_east_counts; }

  long getCryoWestCounts() const
  { return info.cryo2_west_counts; }
  */

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
  uint64_t getLastTimestampCalib() const
  { return Metadata()->getLastTimestampCalib(); }
  uint64_t getLastTimestampOther() const
  { return Metadata()->getLastTimestampOther(); }

  int getLastTriggerType() const
  { return Metadata()->getLastTriggerType(); }
  long getLastTriggerBNB() const
  { return Metadata()->getLastTriggerBNB(); }
  long getLastTriggerNuMI() const
  { return Metadata()->getLastTriggerNuMI(); }
  long getLastTriggerBNBOff() const
  { return Metadata()->getLastTriggerBNBOff(); }
  long getLastTriggerNuMIOff() const
  { return Metadata()->getLastTriggerNuMIOff(); }
  long getLastTriggerCalib() const
  { return Metadata()->getLastTriggerCalib(); }

  long getTotalTriggerBNB() const
  { return Metadata()->getTotalTriggerBNB(); }
  long getTotalTriggerNuMI() const
  { return Metadata()->getTotalTriggerNuMI(); }
  long getTotalTriggerBNBOff() const
  { return Metadata()->getTotalTriggerBNBOff(); }
  long getTotalTriggerNuMIOff() const
  { return Metadata()->getTotalTriggerNuMIOff(); }
  long getTotalTriggerCalib() const
  { return Metadata()->getTotalTriggerCalib(); }
  
  long getDeltaGatesBNB() const
  { return Metadata()->getDeltaGatesBNB(); }
  long getDeltaGatesNuMI() const
  { return Metadata()->getDeltaGatesNuMI(); }
  long getDeltaGatesBNBOff() const
  { return Metadata()->getDeltaGatesBNBOff(); }
  long getDeltaGatesNuMIOff() const
  { return Metadata()->getDeltaGatesNuMIOff(); }
  long getDeltaGatesCalib() const
  { return Metadata()->getDeltaGatesCalib(); }
  long getDeltaGatesOther() const
  { return Metadata()->getDeltaGatesOther(); }

  bool Verify() const;
  
private:
  artdaq::Fragment const & fFragment;
  ICARUSTriggerInfo info;
};

#endif /* sbndaq_datatypes_Overlays_ICARUSTriggerV2Fragment_hh */
