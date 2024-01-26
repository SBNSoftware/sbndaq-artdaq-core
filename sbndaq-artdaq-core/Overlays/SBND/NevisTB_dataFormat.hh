#ifndef sbndaq_Overlays_SBND_NevisTB_dataFormat_hh
#define sbndaq_Overlays_SBND_NevisTB_dataFormat_hh

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"

namespace sbndaq {

  struct NevisTrigger_Header;
  struct NevisTrigger_Data;
  struct NevisTrigger_Trailer;


  //  enum class NevisTBWordType_t;

  //typedef uint16_t                     NevisTB_word_t;
  //typedef std::vector<NevisTB_word_t>  NevisTB_Data_t;

}

/** Trigger Header Words.                                                                                                                                                                     
    4 16-bit words.                                                                                                                                                                           
    (1) 2 MHz Sample number and 16 MHz Remainder                                                                                                                                              
    (2) Frame number low bits                                                                                                                                                                 
    (3) Trigger number low bits and frame number high bits                                                                                                                                    
    (4) Trigger number low bits                                                                                                                                                               
**/

struct sbndaq::NevisTrigger_Header final
{

  //first word                                                                                                                                                                                
  union{
    struct{
      uint16_t busy      :1;
      uint16_t remainder :3;
      uint16_t sample    :12;
    };
    uint16_t trig_time=0xDEAD;
  } ;
  bool getBusyFlag() const noexcept{
    return busy;
  }
  uint16_t getSampleNumber() const noexcept{
    return sample;
  }
  uint16_t get2MHzSampleNumber() const noexcept{
    return sample;
  }
  uint16_t get16MHzRemainderNumber() const noexcept{
    return remainder;
  }

  //second word                                                                                                                                                                               
  union {
    struct {
      uint16_t frame1 :16;
    } ;
    uint16_t frame_1=0xDEAD;
  } ;

  //third word                                                                                                                                                                                
  union {
    struct {
      uint16_t frame2  :8;
      uint16_t ntrig1  :8;
    } ;
    uint16_t frame_2_and_ntrig_1=0xDEAD;
  } ;
  uint32_t getFrame() const noexcept{
    return frame1+(frame2<<16);
  }


  //fourth word                                                                                                                                                                               
  union {
    struct {
      uint16_t ntrig2  :16;
    } ;
    uint16_t ntrig_2=0xDEAD;
  } ;
  uint32_t getTriggerNumber() const noexcept{
    return ntrig1+(ntrig2<<8);
  }

  std::string debugInfo()const noexcept;
  size_t      getWordCount() const noexcept;
  size_t      getModule() const noexcept { return 0; };

};

//static_assert (sizeof(sbndaq::Trigger_Header) == 8, "SBND Trigger_Header structure size is not correct.");                                                                                  


/** Trigger Data Words.                                                                                                                                                                       
    2 16-bit words.                                                                                                                                                                           
    (1) Trigger bit mask 1                                                                                                                                                                    
    (2) Trigger bit mask 2                                                                                                                                                                    
**/

struct sbndaq::NevisTrigger_Data final
{
  //first word                                                                                                                                                                                
  union {
    struct {
      uint16_t pmt_trig_data :8;
      uint16_t pc            :1;
      uint16_t external      :1;
      uint16_t active        :1;
      uint16_t gate2         :1;
      uint16_t gate1         :1;
      uint16_t veto          :1;
      uint16_t calib         :1;
      uint16_t phase0        :1;
    } ;
    uint16_t trig_data_1=0x0;
  } ;
  uint8_t getPMTTrigData() const noexcept{
    return pmt_trig_data;
  }
  bool Trig_PMTBeam() const noexcept{
    return ( (pmt_trig_data&0x01) == 0x01);
  }
  bool Trig_PMTCosmic() const noexcept{
    return ( (pmt_trig_data&0x02) == 0x02);
  }
  bool Trig_PC() const noexcept{
    return pc;
  }
  bool Trig_EXT() const noexcept{
    return external;
  }
  bool Trig_Active() const noexcept{
    return active;
  }
  bool Trig_Gate2() const noexcept{
    return gate2;
  }
  bool Trig_Gate1() const noexcept{
    return gate1;
  }
  bool Trig_Veto() const noexcept{
    return veto;
  }
  bool Trig_Calib() const noexcept{
    return calib;
  }
  //second word                                                                                                                                                                               
  union {
    struct {
      uint16_t phase1        :1;
      uint16_t gatefake      :1;
      uint16_t beamfake      :1;
      uint16_t spare1        :1;
    uint16_t               :12;
    } ;
    uint16_t trig_data_2=0xFFF0;
  } ;
  uint8_t getPhase() const noexcept{
    return phase0+(phase1<<1);
  }
  bool Trig_GateFake() const noexcept{
    return gatefake;
  }
  bool Trig_BeamFake() const noexcept{
    return beamfake;
  }
  bool Trig_Spare1() const noexcept{
    return spare1;
  }
  std::string debugInfo()const noexcept;
} ;

struct sbndaq::NevisTrigger_Trailer final
{
  //first word                                                                                                                                                                                
  union {
    struct {
      uint16_t marker1    :16;
    } ;
    uint16_t trailer1=0xDEAD;
  } ;
  uint16_t getMarker1() const noexcept{
    return marker1;
  }
  //second word                                                                                                                                                                               
  union {
    struct {
      uint16_t marker2    :16;
    } ;
    uint16_t trailer2=0xDEAD;
  } ;
  uint16_t getMarker2() const noexcept{
    return marker2;
  }
  uint32_t getTrailerWord() const noexcept{
    return (trailer1 + (trailer2<<16));
  }
  std::string debugInfo()const noexcept;
} ;

#pragma GCC diagnostic pop

#endif /* sbndaq_datatypes_Overlays_SBND_NevisTBFragment_hh*/


