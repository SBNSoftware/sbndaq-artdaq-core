#ifndef sbndaq_artdaq_core_Overlays_SBND_NevisTPC_NevisTPCTypes_hh
#define sbndaq_artdaq_core_Overlays_SBND_NevisTPC_NevisTPCTypes_hh

#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>

namespace sbndaq{
  
  struct NevisTPCHeader;
  std::ostream & operator << (std::ostream &, NevisTPCHeader const &);
  
  enum class NevisTPCWordType_t;

  typedef uint16_t                     NevisTPC_ADC_t;
  typedef std::vector<NevisTPC_ADC_t>  NevisTPC_Data_t;
}

struct sbndaq::NevisTPCHeader{
  
  union {
    struct {
      uint32_t headerstart	: 16;
      uint32_t slot			: 5; // Slot in crate
      uint32_t FEMID			: 4; // Writable id (e.g. for crate number)
      uint32_t testmode			: 1; // Test mode flag
      uint32_t overflow			: 1; // DRAM oveflow flag
      uint32_t full			: 1; // Header FIFO full flag
    uint32_t				: 4;
    } id_and_slot_word;
    uint32_t id_and_slot = 0xDEADBEEF;
  };
  
  uint8_t getFEMID() const{
    return  (uint8_t) id_and_slot_word.FEMID;
  }
  uint8_t getSlot() const{
    return (uint8_t) id_and_slot_word.slot;
  }
  
  union {
    struct {
      uint32_t wc2			: 12;
    uint32_t				: 4;
      uint32_t wc1			: 12;
    uint32_t 				: 4;
    } word_count_word;
    uint32_t word_count = 0xDEADBEEF;
  };
  
  uint32_t getADCWordCount() const {
    return (word_count_word.wc2<<12 | word_count_word.wc1);
  }
  
  union {
    struct {
      uint32_t en2			: 12;
    uint32_t				: 4;
      uint32_t en1			: 12;
    uint32_t				: 4;
    } event_num_word;
    uint32_t event_num = 0xDEADBEEF;
  };
  
  uint32_t getEventNum() const {
    return  event_num_word.en2<<12 | event_num_word.en1;
  }
  
  union {
    struct {
      uint32_t fn2			: 12;
    uint32_t				: 4;
      uint32_t fn1			: 12;
    uint32_t				: 4;
    } frame_num_word;
    uint32_t frame_num = 0xDEADBEEF;
  };
  
  uint32_t getFrameNum() const {
    return frame_num_word.fn2 << 12 | frame_num_word.fn1;
  }
  
  union {
    struct {
      uint32_t cs2			: 12;
    uint32_t 				: 4;
      uint32_t cs1			: 12;
    uint32_t 				: 4;
    } checksum_word;
    uint32_t checksum = 0xDEADBEEF;
  };
  
  uint32_t getChecksum() const {
    return checksum_word.cs2 << 12 | checksum_word.cs1;
  }
  
  union {
    struct {
      uint32_t tf2			: 4;
      uint32_t tf3			: 4;
    uint32_t				: 8;
      uint32_t tf1			: 8;
    uint32_t				: 8;
    } trig_frame_sample_word;
    uint32_t trig_frame_sample = 0xDEADBEEF;
  };
  
  uint32_t get2mhzSample() const {
    return trig_frame_sample_word.tf2 << 8 | trig_frame_sample_word.tf1;
  }
  uint32_t getTrigFrame() const {
    return trig_frame_sample_word.tf3;
  }
  
  NevisTPCHeader(uint8_t _fid, uint8_t _slot, uint32_t _adcwords, uint32_t _eventnum, uint32_t _framenum, uint32_t _checksum, uint32_t _trigframe, uint32_t _sample2mhznum){
    id_and_slot = 0xFFFF + ((0xF000 | ((_fid & 0xF) << 5) | (_slot & 0b11111)) << 16);
    word_count = (0xF000 | ((_adcwords>>12) & 0xFFF)) + ((0xF000 | (_adcwords & 0xFFF)) << 16);
    event_num = (0xF000 | ((_eventnum>>12) & 0xFFF)) + ((0xF000 | (_eventnum & 0xFFF)) << 16);
    frame_num = (0xF000 | ((_framenum>>12) & 0xFFF)) + ((0xF000 | (_framenum & 0xFFF)) << 16);
    checksum = (0xF000 | ((_checksum>>12) & 0xFFF)) + ((0xF000 | (_checksum & 0xFFF)) << 16);
    trig_frame_sample = (0xF000 | (_trigframe & 0xF << 4) | ((_sample2mhznum >> 8) & 0xF)) + ((0xF000 | (_sample2mhznum & 0xFFF)) << 16);
  }
  NevisTPCHeader(){};
  
  const char* c_str() const { std::ostringstream ss; ss << *this; return ss.str().c_str(); }
};

enum class sbndaq::NevisTPCWordType_t
{
  kChannelHeader,
  kADC,
  kADCHuffman,
  kChannelEnding,
  kUnknown
};

#endif
