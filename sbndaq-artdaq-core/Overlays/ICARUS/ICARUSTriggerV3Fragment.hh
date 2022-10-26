#ifndef sbndaq_artdaq_core_Overlays_ICARUS_ICARUSTriggerV3Fragment_hh
#define sbndaq_artdaq_core_Overlays_ICARUS_ICARUSTriggerV3Fragment_hh

#include "artdaq-core/Data/Fragment.hh"
#include "sbndaq-artdaq-core/Overlays/ICARUS/ICARUSTriggerInfo.hh"
#include "sbndaq-artdaq-core/Trace/trace_defines.h"
#include "cetlib_except/exception.h"

#include <iostream>
#include <chrono>
#include <string>
#include <map>


namespace icarus {

  struct ICARUSTriggerInfo;
  ICARUSTriggerInfo parse_ICARUSTriggerV3String(const char*);
  class ICARUSTriggerV3Fragment;
  std::ostream & operator << (std::ostream &, ICARUSTriggerV3Fragment const &);

  class ICARUSTriggerV3FragmentMetadata;
  //std::ostream & operator << (std::ostream &, ICARUSTriggerV3FragmentMetadata const &);
}

inline icarus::ICARUSTriggerInfo icarus::parse_ICARUSTriggerV3String(const char* buffer)
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


class icarus::ICARUSTriggerV3FragmentMetadata
{

public:  
  ICARUSTriggerV3FragmentMetadata() {}
  ICARUSTriggerV3FragmentMetadata(uint64_t ntp_t, 
				   uint64_t last_ts, 
				   uint64_t last_ts_bnbmaj, uint64_t last_ts_bnbminbias, 
				   uint64_t last_ts_numimaj, uint64_t last_ts_numiminbias, 
				   uint64_t last_ts_bnboffmaj, uint64_t last_ts_bnboffminbias,
				   uint64_t last_ts_numioffmaj, uint64_t last_ts_numioffminbias,
				   uint64_t last_ts_calibmaj, uint64_t last_ts_calibminbias, 
				   uint64_t last_ts_other,
				   int last_trig_type, int last_sou_type, 
				   long last_trig_bnbmaj, long last_trig_bnbminbias, 
				   long last_trig_numimaj, long last_trig_numiminbias, 
				   long last_trig_bnboffmaj, long last_trig_bnboffminbias,
				   long last_trig_numioffmaj, long last_trig_numioffminbias, 
				   long last_trig_calibmaj, long last_trig_calibminbias,
				   long total_trig_bnb, long total_trig_bnbmaj, long total_trig_bnbminbias, 
				   long total_trig_numi, long total_trig_numimaj, long total_trig_numiminbias, 
				   long total_trig_bnboff, long total_trig_bnboffmaj, long total_trig_bnboffminbias,
				   long total_trig_numioff, long total_trig_numioffmaj, 
				   long total_trig_numioffminbias, long total_trig_calibmaj,
				   long total_trig_calibminbias, long dg,
				   long dg_bnbmaj, long dg_bnbminbias, 
				   long dg_numimaj, long dg_numiminbias, 
				   long dg_bnboffmaj, long dg_bnboffminbias, 
				   long dg_numioffmaj, long dg_numioffminbias,
				   long dg_calibmaj, long dg_calibminbias, long dg_other,
				   long totalgates_bnbmaj, long totalgates_bnbminbias,
				   long totalgates_numimaj, long totalgates_numiminbias,
				   long totalgates_bnboffmaj, long totalgates_bnboffminbias,
				   long totalgates_numioffmaj, long totalgates_numioffminbias,
				   long totalgates_calibmaj, long totalgates_calibminbias) 
    : ntp_time(ntp_t)
    , last_timestamp(last_ts)
    , last_timestamp_bnbmaj(last_ts_bnbmaj)
    , last_timestamp_bnbminbias(last_ts_bnbminbias)
    , last_timestamp_numimaj(last_ts_numimaj)
    , last_timestamp_numiminbias(last_ts_numiminbias)
    , last_timestamp_bnboffmaj(last_ts_bnboffmaj)
    , last_timestamp_bnboffminbias(last_ts_bnboffminbias)
    , last_timestamp_numioffmaj(last_ts_numioffmaj)
    , last_timestamp_numioffminbias(last_ts_numioffminbias)
    , last_timestamp_calibmaj(last_ts_calibmaj)
    , last_timestamp_calibminbias(last_ts_calibminbias)
    , last_timestamp_other(last_ts_other)
    , last_trigger_type(last_trig_type)
    , last_source_type(last_sou_type)
    , last_trigger_bnbmaj(last_trig_bnbmaj)
    , last_trigger_bnbminbias(last_trig_bnbminbias)
    , last_trigger_numimaj(last_trig_numimaj)
    , last_trigger_numiminbias(last_trig_numiminbias)
    , last_trigger_bnboffmaj(last_trig_bnboffmaj)
    , last_trigger_bnboffminbias(last_trig_bnboffminbias)
    , last_trigger_numioffmaj(last_trig_numioffmaj)
    , last_trigger_numioffminbias(last_trig_numioffminbias)
    , last_trigger_calibmaj(last_trig_calibmaj)
    , last_trigger_calibminbias(last_trig_calibminbias)
    , total_trigger_bnb(total_trig_bnb)
    , total_trigger_bnbmaj(total_trig_bnbmaj)
    , total_trigger_bnbminbias(total_trig_bnbminbias)
    , total_trigger_numi(total_trig_numi)
    , total_trigger_numimaj(total_trig_numimaj)
    , total_trigger_numiminbias(total_trig_numiminbias)
    , total_trigger_bnboff(total_trig_bnboff)
    , total_trigger_bnboffmaj(total_trig_bnboffmaj)
    , total_trigger_bnboffminbias(total_trig_bnboffminbias)
    , total_trigger_numioff(total_trig_numioff)
    , total_trigger_numioffmaj(total_trig_numioffmaj)
    , total_trigger_numioffminbias(total_trig_numioffminbias)
    , total_trigger_calibmaj(total_trig_calibmaj)
    , total_trigger_calibminbias(total_trig_calibminbias)
    , delta_gates(dg) 
    , delta_gates_bnbmaj(dg_bnbmaj)
    , delta_gates_bnbminbias(dg_bnbminbias)
    , delta_gates_numimaj(dg_numimaj)
    , delta_gates_numiminbias(dg_numiminbias)
    , delta_gates_bnboffmaj(dg_bnboffmaj)
    , delta_gates_bnboffminbias(dg_bnboffminbias)
    , delta_gates_numioffmaj(dg_numioffmaj)
    , delta_gates_numioffminbias(dg_numioffminbias)
    , delta_gates_calibmaj(dg_calibmaj)
    , delta_gates_calibminbias(dg_calibminbias)
    , delta_gates_other(dg_other) 
    , total_gates_bnbmaj(totalgates_bnbmaj)
    , total_gates_bnbminbias(totalgates_bnbminbias)
    , total_gates_numimaj(totalgates_numimaj)
    , total_gates_numiminbias(totalgates_numiminbias)
    , total_gates_bnboffmaj(totalgates_bnboffmaj)
    , total_gates_bnboffminbias(totalgates_bnboffminbias)
    , total_gates_numioffmaj(totalgates_numioffmaj)
    , total_gates_numioffminbias(totalgates_numioffminbias)
    , total_gates_calibmaj(totalgates_calibmaj)
    , total_gates_calibminbias(totalgates_calibminbias)
  {}
  
  uint64_t getNTPTime() const
  { return ntp_time; } 

  uint64_t getLastTimestamp() const
  { return last_timestamp; }

  uint64_t getLastTimestampBNBMaj() const
  { return last_timestamp_bnbmaj; }

  uint64_t getLastTimestampBNBMinbias() const
  { return last_timestamp_bnbminbias; }
  
  uint64_t getLastTimestampNuMIMaj() const
  { return last_timestamp_numimaj; }

  uint64_t getLastTimestampNuMIMinbias() const
  { return last_timestamp_numiminbias; }

  uint64_t getLastTimestampBNBOffMaj() const
  { return last_timestamp_bnboffmaj; }
  
  uint64_t getLastTimestampBNBOffMinbias() const
  { return last_timestamp_bnboffminbias; }
  
  uint64_t getLastTimestampNuMIOffMaj() const
  { return last_timestamp_numioffmaj; }
  
  uint64_t getLastTimestampNuMIOffMinbias() const
  { return last_timestamp_numioffminbias; }

  uint64_t getLastTimestampCalibMaj() const
  { return last_timestamp_calibmaj; }
  
  uint64_t getLastTimestampCalibMinbias() const
  { return last_timestamp_calibminbias; }
    
  uint64_t getLastTimestampOther() const
  { return last_timestamp_other; }

  int getLastTriggerType() const
  { return last_trigger_type; }

  int getLastSourceType() const
  { return last_source_type; }

  long getLastTriggerBNBMaj() const
  { return last_trigger_bnbmaj; }

  long getLastTriggerBNBMinbias() const
  { return last_trigger_bnbminbias; }
  
  long getLastTriggerNuMIMaj() const
  { return last_trigger_numimaj; }

  long getLastTriggerNuMIMinbias() const
  { return last_trigger_numiminbias; }
  
  long getLastTriggerBNBOffMaj() const
  { return last_trigger_bnboffmaj; }

  long getLastTriggerBNBOffMinbias() const
  { return last_trigger_bnboffminbias; }

  long getLastTriggerNuMIOffMaj() const
  { return last_trigger_numioffmaj; }

  long getLastTriggerNuMIOffMinbias() const
  { return last_trigger_numioffminbias; }

  long getLastTriggerCalibMaj() const
  { return last_trigger_calibmaj; }

  long getLastTriggerCalibMinbias() const
  { return last_trigger_calibminbias; }

  long getTotalTriggerBNB() const
  { return total_trigger_bnb; }

  long getTotalTriggerBNBMaj() const
  { return total_trigger_bnbmaj; }

  long getTotalTriggerBNBMinbias() const
  { return total_trigger_bnbminbias; }

  long getTotalTriggerNuMI() const
  { return total_trigger_numi; }

  long getTotalTriggerNuMIMaj() const
  { return total_trigger_numimaj; }
  
  long getTotalTriggerNuMIMinbias() const
  { return total_trigger_numiminbias; }

  long getTotalTriggerBNBOff() const
  { return total_trigger_bnboff; }

  long getTotalTriggerBNBOffMaj() const
  { return total_trigger_bnboffmaj; }

  long getTotalTriggerBNBOffMinbias() const
  { return total_trigger_bnboffminbias; }

  long getTotalTriggerNuMIOff() const
  { return total_trigger_numioff; }

  long getTotalTriggerNuMIOffMaj() const
  { return total_trigger_numioffmaj; }

  long getTotalTriggerNuMIOffMinbias() const
  { return total_trigger_numioffminbias; }

  long getTotalTriggerCalibMaj() const
  { return total_trigger_calibmaj; }

  long getTotalTriggerCalibMinbias() const
  { return total_trigger_calibminbias; }

  long getDeltaGates() const
  { return delta_gates; }

  long getDeltaGatesBNBMaj() const
  { return delta_gates_bnbmaj; }

  long getDeltaGatesBNBMinbias() const
  { return delta_gates_bnbminbias; }

  long getDeltaGatesNuMIMaj() const
  { return delta_gates_numimaj; }

  long getDeltaGatesNuMIMinbias() const
  { return delta_gates_numiminbias; }

  long getDeltaGatesBNBOffMaj() const
  { return delta_gates_bnboffmaj; }

  long getDeltaGatesBNBOffMinbias() const
  { return delta_gates_bnboffminbias; }
  
  long getDeltaGatesNuMIOffMaj() const
  { return delta_gates_numioffmaj; }

  long getDeltaGatesNuMIOffMinbias() const
  { return delta_gates_numioffminbias; }

  long getDeltaGatesCalibMaj() const
  { return delta_gates_calibmaj; }

  long getDeltaGatesCalibMinbias() const
  { return delta_gates_calibminbias; }

  long getDeltaGatesOther() const
  { return delta_gates_other; }

  long getTotalGatesBNBMaj() const
  { return total_gates_bnbmaj; }
  
  long getTotalGatesBNBMinbias() const
  { return total_gates_bnbminbias; }

  long getTotalGatesNuMIMaj() const
  { return total_gates_numimaj; }

  long getTotalGatesNuMIMinbias() const
  { return total_gates_numiminbias; }

  long getTotalGatesBNBOffMaj() const
  { return total_gates_bnboffmaj; }

  long getTotalGatesBNBOffMinbias() const
  { return total_gates_bnboffminbias; }

  long getTotalGatesNuMIOffMaj() const
  { return total_gates_numioffmaj; }

  long getTotalGatesNuMIOffMinbias() const
  { return total_gates_numioffminbias; }

  long getTotalGatesCalibMaj() const
  { return total_gates_calibmaj; }

  long getTotalGatesCalibMinbias() const
  { return total_gates_calibminbias; }

private:
  uint64_t ntp_time;
  uint64_t last_timestamp;
  uint64_t last_timestamp_bnbmaj;
  uint64_t last_timestamp_bnbminbias;
  uint64_t last_timestamp_numimaj;
  uint64_t last_timestamp_numiminbias;
  uint64_t last_timestamp_bnboffmaj;
  uint64_t last_timestamp_bnboffminbias;
  uint64_t last_timestamp_numioffmaj;
  uint64_t last_timestamp_numioffminbias;
  uint64_t last_timestamp_calibmaj;
  uint64_t last_timestamp_calibminbias;
  uint64_t last_timestamp_other;

  int last_trigger_type;
  int last_source_type;
  long last_trigger_bnbmaj;
  long last_trigger_bnbminbias;
  long last_trigger_numimaj;
  long last_trigger_numiminbias;
  long last_trigger_bnboffmaj;
  long last_trigger_bnboffminbias;
  long last_trigger_numioffmaj;
  long last_trigger_numioffminbias;
  long last_trigger_calibmaj;
  long last_trigger_calibminbias;
  
  long total_trigger_bnb;
  long total_trigger_bnbmaj;
  long total_trigger_bnbminbias;
  long total_trigger_numi;
  long total_trigger_numimaj;
  long total_trigger_numiminbias;
  long total_trigger_bnboff;
  long total_trigger_bnboffmaj;
  long total_trigger_bnboffminbias;
  long total_trigger_numioff;
  long total_trigger_numioffmaj;
  long total_trigger_numioffminbias;
  long total_trigger_calibmaj;
  long total_trigger_calibminbias;

  long delta_gates;
  long delta_gates_bnbmaj;
  long delta_gates_bnbminbias;
  long delta_gates_numimaj;
  long delta_gates_numiminbias;
  long delta_gates_bnboffmaj;
  long delta_gates_bnboffminbias;
  long delta_gates_numioffmaj;
  long delta_gates_numioffminbias;
  long delta_gates_calibmaj;
  long delta_gates_calibminbias;
  long delta_gates_other;

  long total_gates_bnbmaj;
  long total_gates_bnbminbias;
  long total_gates_numimaj;
  long total_gates_numiminbias;
  long total_gates_bnboffmaj;
  long total_gates_bnboffminbias;
  long total_gates_numioffmaj;
  long total_gates_numioffminbias;
  long total_gates_calibmaj;
  long total_gates_calibminbias;


};

class icarus::ICARUSTriggerV3Fragment{

public:
  
  ICARUSTriggerV3Fragment(artdaq::Fragment const &f) : fFragment(f) {info = parse_ICARUSTriggerV3String((const char*)fFragment.dataBeginBytes());}
  ICARUSTriggerV3FragmentMetadata const * Metadata() const
  { return fFragment.metadata<ICARUSTriggerV3FragmentMetadata>(); }
  
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

  uint64_t getLastTimestampBNBMaj() const
  { return Metadata()->getLastTimestampBNBMaj(); }
  uint64_t getLastTimestampBNBMinbias() const
  { return Metadata()->getLastTimestampBNBMinbias(); }
  uint64_t getLastTimestampNuMIMaj() const
  { return Metadata()->getLastTimestampNuMIMaj(); }
  uint64_t getLastTimestampNuMIMinbias() const
  { return Metadata()->getLastTimestampNuMIMinbias(); }
  uint64_t getLastTimestampBNBOffMaj() const
  { return Metadata()->getLastTimestampBNBOffMaj(); }
  uint64_t getLastTimestampBNBOffMinbias() const
  { return Metadata()->getLastTimestampBNBOffMinbias(); }
  uint64_t getLastTimestampNuMIOffMaj() const
  { return Metadata()->getLastTimestampNuMIOffMaj(); }
  uint64_t getLastTimestampNuMIOffMinbias() const
  { return Metadata()->getLastTimestampNuMIOffMinbias(); }
  uint64_t getLastTimestampCalibMaj() const
  { return Metadata()->getLastTimestampCalibMaj(); }
  uint64_t getLastTimestampCalibMinbias() const
  { return Metadata()->getLastTimestampCalibMinbias(); }
  uint64_t getLastTimestampOther() const
  { return Metadata()->getLastTimestampOther(); }

  int getLastTriggerType() const
  { return Metadata()->getLastTriggerType(); }
  int getLastSourceType() const
  { return Metadata()->getLastSourceType(); }
  long getLastTriggerBNBMaj() const
  { return Metadata()->getLastTriggerBNBMaj(); }
  long getLastTriggerBNBMinbias() const
  { return Metadata()->getLastTriggerBNBMinbias(); }
  long getLastTriggerNuMIMaj() const
  { return Metadata()->getLastTriggerNuMIMaj(); }
  long getLastTriggerNuMIMinbias() const
  { return Metadata()->getLastTriggerNuMIMinbias(); }
  long getLastTriggerBNBOffMaj() const
  { return Metadata()->getLastTriggerBNBOffMaj(); }
  long getLastTriggerBNBOffMinbias() const
  { return Metadata()->getLastTriggerBNBOffMinbias(); }
  long getLastTriggerNuMIOffMaj() const
  { return Metadata()->getLastTriggerNuMIOffMaj(); }
  long getLastTriggerNuMIOffMinbias() const
  { return Metadata()->getLastTriggerNuMIOffMinbias(); }
  long getLastTriggerCalibMaj() const
  { return Metadata()->getLastTriggerCalibMaj(); }
  long getLastTriggerCalibMinbias() const
  { return Metadata()->getLastTriggerCalibMinbias(); }

  long getTotalTrigger() const
  { return Metadata()->getTotalTriggerBNB(); }
  long getTotalTriggerBNBMaj() const
  { return Metadata()->getTotalTriggerBNBMaj(); }
  long getTotalTriggerBNBMinbias() const
  { return Metadata()->getTotalTriggerBNBMinbias(); }
  long getTotalTriggerNuMI() const
  { return Metadata()->getTotalTriggerNuMI(); }
  long getTotalTriggerNuMIMaj() const
  { return Metadata()->getTotalTriggerNuMIMaj(); }
  long getTotalTriggerNuMIMinbias() const
  { return Metadata()->getTotalTriggerNuMIMinbias(); }
  long getTotalTriggerBNBOff() const
  { return Metadata()->getTotalTriggerBNBOff(); }
  long getTotalTriggerBNBOffMaj() const
  { return Metadata()->getTotalTriggerBNBOffMaj(); }
  long getTotalTriggerBNBOffMinbias() const
  { return Metadata()->getTotalTriggerBNBOffMinbias(); }
  long getTotalTriggerNuMIOff() const
  { return Metadata()->getTotalTriggerNuMIOff(); }
  long getTotalTriggerNuMIOffMaj() const
  { return Metadata()->getTotalTriggerNuMIOffMaj(); }
  long getTotalTriggerNuMIOffMinbias() const
  { return Metadata()->getTotalTriggerNuMIOffMinbias(); }
  long getTotalTriggerCalibMaj() const
  { return Metadata()->getTotalTriggerCalibMaj(); }
  long getTotalTriggerCalibMinbias() const
  { return Metadata()->getTotalTriggerCalibMinbias(); }

  long getDeltaGatesBNBMaj() const
  { return Metadata()->getDeltaGatesBNBMaj(); }
  long getDeltaGatesBNBMinbias() const
  { return Metadata()->getDeltaGatesBNBMinbias(); }
  long getDeltaGatesNuMIMaj() const
  { return Metadata()->getDeltaGatesNuMIMaj(); }
  long getDeltaGatesNuMIMinbias() const
  { return Metadata()->getDeltaGatesNuMIMinbias(); }
  long getDeltaGatesBNBOffMaj() const
  { return Metadata()->getDeltaGatesBNBOffMaj(); }
  long getDeltaGatesBNBOffMinbias() const
  { return Metadata()->getDeltaGatesBNBOffMinbias(); }
  long getDeltaGatesNuMIOffMaj() const
  { return Metadata()->getDeltaGatesNuMIOffMaj(); }
  long getDeltaGatesNuMIOffMinbias() const
  { return Metadata()->getDeltaGatesNuMIOffMinbias(); }
  long getDeltaGatesCalibMaj() const
  { return Metadata()->getDeltaGatesCalibMaj(); }
  long getDeltaGatesCalibMinbias() const
  { return Metadata()->getDeltaGatesCalibMinbias(); }
  long getDeltaGatesOther() const
  { return Metadata()->getDeltaGatesOther(); }

  long getTotalGatesBNBMaj() const
  { return Metadata()->getTotalGatesBNBMaj(); }
  long getTotalGatesBNBMinbias() const
  { return Metadata()->getTotalGatesBNBMinbias(); }
  long getTotalGatesNuMIMaj() const
  { return Metadata()->getTotalGatesNuMIMaj(); }
  long getTotalGatesNuMIMinbias() const
  { return Metadata()->getTotalGatesNuMIMinbias(); }
  long getTotalGatesBNBOffMaj() const
  { return Metadata()->getTotalGatesBNBOffMaj(); }
  long getTotalGatesBNBOffMinbias() const
  { return Metadata()->getTotalGatesBNBOffMinbias(); }
  long getTotalGatesNuMIOffMaj() const
  { return Metadata()->getTotalGatesNuMIOffMaj(); }
  long getTotalGatesNuMIOffMinbias() const
  { return Metadata()->getTotalGatesNuMIOffMinbias(); }
  long getTotalGatesCalibMaj() const
  { return Metadata()->getTotalGatesCalibMaj(); }
  long getTotalGatesCalibMinbias() const
  { return Metadata()->getTotalGatesCalibMinbias(); }

  bool Verify() const;
  
private:
  artdaq::Fragment const & fFragment;
  ICARUSTriggerInfo info;
};

#endif /* sbndaq_datatypes_Overlays_ICARUSTriggerV3Fragment_hh */
