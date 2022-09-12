#ifndef sbndaq_artdaq_core_Overlays_ICARUS_ICARUSTriggerInfo_hh
#define sbndaq_artdaq_core_Overlays_ICARUS_ICARUSTriggerInfo_hh

#include <string>
#include <cstdint>


namespace icarus { struct ICARUSTriggerInfo; }

struct icarus::ICARUSTriggerInfo
{
  int version                =  0;
  std::string name;
  long event_no              = -1;
  long seconds               = -2;
  long nanoseconds           = -3;
  std::string wr_name;
  long wr_event_no           = -1;
  long wr_seconds            = -2;
  long wr_nanoseconds        = -3;
  int enable_type            = -1;
  long enable_seconds        =  0;
  long enable_nanoseconds    =  0;
  long gate_id               = -4;
  long gate_id_BNB           = -4;
  long gate_id_NuMI          = -4;
  long gate_id_BNBOff        = -4;
  long gate_id_NuMIOff       = -4;
  int gate_type              =  0;
  long beam_seconds          =  0;
  long beam_nanoseconds      =  0;
  int trigger_type           =  0;
  int trigger_source         =  0;
  std::string cryo1_e_conn_0;
  std::string cryo1_e_conn_2;
  std::string cryo2_w_conn_0;
  std::string cryo2_w_conn_2;
  long cryo1_east_counts     = -1;
  long cryo2_west_counts     = -1;

  uint64_t getNanoseconds_since_UTC_epoch() const {
    if(wr_seconds == -2 || wr_nanoseconds == -3)
      return 0;
    int correction = 0;
    if(wr_seconds >= 1483228800)
      correction = 37;
     uint64_t const corrected_ts
     { (wr_seconds-correction)*1000000000ULL + wr_nanoseconds };
     return corrected_ts;
  }

}; // icarus::ICARUSTriggerInfo


#endif /* sbndaq_artdaq_core_Overlays_ICARUS_ICARUSTriggerInfo_hh */
