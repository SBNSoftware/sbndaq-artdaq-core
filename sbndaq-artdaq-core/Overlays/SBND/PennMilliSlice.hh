#ifndef sbnddaq_datatypes_Overlays_PennMilliSlice_hh
#define sbnddaq_datatypes_Overlays_PennMilliSlice_hh

#include "sbndaq-artdaq-core/Overlays/SBND/PennMicroSlice.hh"
#include "artdaq-core/Data/Fragment.hh"

#include <boost/crc.hpp>

//#define PENN_DONT_REBLOCK_USLICES
//#define PENN_OLD_STRUCTS

namespace sbnddaq {
  class PennMilliSlice;
}

class sbnddaq::PennMilliSlice {

public:

#ifdef PENN_OLD_STRUCTS
  struct TriggerPayload {
    //FIXME: Change this format to something easier to debug.
    //We'll use 32 bit atom since that is the total length of this struct 
    typedef uint32_t trigger_type_t;
    typedef uint32_t trigger_pattern_t;
    typedef uint16_t trigger_bits_t;
    trigger_pattern_t trigger_pattern: 27;
    trigger_type_t trigger_type      :  5;

    static trigger_bits_t const num_bits_trigger_pattern = 27;
    static trigger_bits_t const num_bits_trigger_type    =  5;

    struct TriggerPatternBits {
      static trigger_bits_t const bsu_rm_cl = 26;
      static trigger_bits_t const tsu_nu_sl = 25;
      static trigger_bits_t const tsu_sl_nu = 24;
      static trigger_bits_t const tsu_el_wu = 23;
      static std::string getName(trigger_bits_t trigger_bit){
	switch(trigger_bit){
	case bsu_rm_cl:
	  return "bsu_rm_cl";
	  break;
	case tsu_nu_sl:
	  return "tsu_nu_sl";
	  break;
	case tsu_sl_nu:
	  return "tsu_sl_nu";
	  break;
	case tsu_el_wu:
	  return "tsu_el_wu";
	  break;
	default:
	  return "unkown";
	  break;
	}//switch(trigger_bit
	return "";
      }
    };

    struct TriggerTypes {
      static trigger_type_t const calibration = 0x00;
      //FIXME not sure about the rce stuff!
      static trigger_type_t const rce_a = 0x01;
      static trigger_type_t const rce_b = 0x02;
      static trigger_type_t const rce_c = 0x03;
      static trigger_type_t const rce_d = 0x04;
      static trigger_type_t const rce_e = 0x05;
      static trigger_type_t const rce_f = 0x06;
      static trigger_type_t const rce_g = 0x07;
      static trigger_type_t const ssp   = 0x08;
      static trigger_type_t const muon  = 0x10;
      static std::string getName(trigger_type_t trigger_type){
	switch(trigger_type){
	case calibration:
	  return "calibration";
	  break;
	case rce_a:
	  return "rce_a";
	  break;
	case rce_b:
	  return "rce_b";
	  break;
	case rce_c:
	  return "rce_c";
	  break;
	case rce_d:
	  return "rce_d";
	  break;
	case rce_e:
	  return "rce_e";
	  break;
	case rce_f:
	  return "rce_f";
	  break;
	case rce_g:
	  return "rce_g";
	  break;
	case ssp  :
	  return "ssp";
	  break;
	case muon :
	  return "muon";
	  break;
	default:
	  return "unknown";
	  break;
	}
	return "";
      }//getName
    };

  };

  struct TimestampPayload {
      typedef uint64_t timestamp_t;
      typedef uint16_t data_size_t;
      timestamp_t nova_timestamp : 64;

      static data_size_t const num_bits_timestamp = 64;
      static size_t const size_words = sizeof(uint64_t);

      static size_t const ptb_offset = sizeof(uint32_t);

  };

  struct CounterPayload {
    //Need to be careful about choice of atom within this struct due to atom boundaries of the bitfields
    //The 128 bits can be devided into two 64 bit atoms, so we will use that
    typedef uint64_t counter_set_t;
    typedef uint16_t trigger_bits_t;
    counter_set_t tsu_wu     : 10;
    counter_set_t tsu_el     : 10;
    counter_set_t tsu_extra  :  4;    
    counter_set_t tsu_nu     :  6;
    counter_set_t tsu_sl     :  6;
    counter_set_t tsu_nl     :  6;
    counter_set_t tsu_su     :  6;
    counter_set_t bsu_rm     : 16;//end of first counter_set_t==uint64_t
    counter_set_t bsu_cu     : 10;
    counter_set_t bsu_cl     : 13;
    counter_set_t bsu_rl     : 10;
    counter_set_t ts_rollover: 28;
    counter_set_t header     :  3;//end of second counter_set_t==uint64_t
    static trigger_bits_t const num_bits_tsu_wu     = 10;
    static trigger_bits_t const num_bits_tsu_el     = 10;
    static trigger_bits_t const num_bits_tsu_extra  =  4;
    static trigger_bits_t const num_bits_tsu_nu     =  6;
    static trigger_bits_t const num_bits_tsu_sl     =  6;
    static trigger_bits_t const num_bits_tsu_nl     =  6;
    static trigger_bits_t const num_bits_tsu_su     =  6;
    static trigger_bits_t const num_bits_bsu_rm     = 16;
    static trigger_bits_t const num_bits_bsu_cu     = 10;
    static trigger_bits_t const num_bits_bsu_cl     = 13;
    static trigger_bits_t const num_bits_bsu_rl     = 10;
    static trigger_bits_t const num_bits_ts_rollover= 28;
    static trigger_bits_t const num_bits_header     =  3;
  };
#endif /* PENN_OLD_STRUCTS */
  struct Header {
    //typedef uint32_t data_t;
    typedef uint64_t data_t;

    typedef uint16_t sequence_id_t;
    typedef uint16_t version_t;
    typedef uint32_t millislice_size_t;  
    typedef uint16_t payload_count_t;
    typedef uint64_t timestamp_t;
    typedef uint32_t ticks_t;
#ifdef PENN_DONT_REBLOCK_USLICES
    typedef uint32_t microslice_count_t;
#endif

    // TODO finalise millislice header

    data_t sequence_id     : 16;
    data_t version         : 16;
    data_t millislice_size : 32;   // total size, data & header
#ifdef PENN_DONT_REBLOCK_USLICES
    data_t microslice_count : 32;
#endif

    data_t payload_count           : 16;
    data_t payload_count_counter   : 16;
    data_t payload_count_trigger   : 16;
    data_t payload_count_timestamp : 16;
// -- There is information missing here
#ifdef NEW_FORMAT
    data_t payload_count_warning  : 16;
#endif
    data_t end_timestamp    : 64;
    data_t width_in_ticks   : 32;  // neglecting overlap
    data_t overlap_in_ticks : 32;
  };


  // This constructor accepts a memory buffer that contains an existing
  // PennMilliSlice and allows the the data inside it to be accessed
  PennMilliSlice(uint8_t* address);

  // Returns the size of the MilliSlice
  Header::millislice_size_t size() const;

  // Returns the sequence ID of the MilliSlice
  Header::sequence_id_t sequenceID() const;

  // Returns the version of the MilliSlice
  Header::version_t version() const;

  // Returns the timestamp marking the end of the MilliSlice
  Header::timestamp_t endTimestamp() const;

  // Returns the number of ticks (neglecting overlaps) in the MilliSlice
  Header::ticks_t widthTicks() const;

  // Returns the number of ticks in the overlap region of the MilliSlice
  Header::ticks_t overlapTicks() const;

  // Returns the number of payloads in this MilliSlice
  Header::payload_count_t payloadCount() const;
  Header::payload_count_t payloadCount(Header::payload_count_t& counter, Header::payload_count_t& trigger, Header::payload_count_t& timestamp
				       //, Header::payload_count_t& warning, Header::payload_count_t& checksum
				       // NFB, Jan-15-2016 : checksums should be kept out of the millislice.
				       ) const;


#ifdef PENN_DONT_REBLOCK_USLICES
  // Returns the number of MicroSlices in this MilliSlice
  Header::microslice_count_t microSliceCount() const;

  // Returns the requested MicroSlice if the requested slice was found,
  // otherwise returns an empty pointer
  std::unique_ptr<PennMicroSlice> microSlice(uint32_t index) const;
#endif

  // Returns the requested Payload if found,
  // otherwise returns an empty pointer
  uint8_t* payload(uint32_t index, sbnddaq::PennMicroSlice::Payload_Header::data_packet_type_t& data_packet_type,
                   sbnddaq::PennMicroSlice::Payload_Header::short_nova_timestamp_t& short_nova_timestamp,
                   size_t& payload_size) const;

//  // Returns a requested payload. The header is passed by reference. A pointer to the begin of the body
//  // is returned.
//  // if the payload is not found a null pointer is returned.
//  uint8_t* payload(uint32_t index, sbnddaq::PennMicroSlice::Payload_Header &data_header, size_t &payload_size) const;

  // Returns a requested payload. The header is passed by reference. A pointer to the begin of the body
  // is returned.
  // if the payload is not found a null pointer is returned.
  // NFB: approved
  uint8_t* payload(uint32_t index, sbnddaq::PennMicroSlice::Payload_Header* &data_header) const;

  // Returns the next payload if found, and increments the current_payload_ buffer, current_word_id_
  // Returns nullptr if there is a problem or we run off the end of the buffer_
  // Can't be const since it shifts the current_payload_buffer and current_word_id_
  uint8_t* get_next_payload(uint32_t &index, 
                            sbnddaq::PennMicroSlice::Payload_Header::data_packet_type_t& data_packet_type,
                            sbnddaq::PennMicroSlice::Payload_Header::short_nova_timestamp_t& short_nova_timestamp,
                            size_t& payload_size); 

  //NFB: My own implementation of get_next_payload
  // return nullpltr if runs off the buffer
  // or if there is some other unspecified problem
  // it also updates the current_payload_buffer and current_word_id_
  uint8_t* get_next_payload(uint32_t &index,sbnddaq::PennMicroSlice::Payload_Header*& data_header);

  // NFB: Returns the nearest timestamp payload after the current_word_id_
  uint8_t* get_next_timestamp(sbnddaq::PennMicroSlice::Payload_Header*& data_header);

#ifdef ENABLE_PENNMILLISLICE_CHECKSUM
  typedef uint32_t checksum_t;

  // Calculate a checksum for this millslice
  checksum_t calculateChecksum() const;

  // Returns the checksum
  checksum_t checksum() const;
#endif

protected:

  // returns a pointer to the header
  Header const* header_() const;

  // returns a pointer to the requested MicroSlice
  uint8_t* data_(int index) const;

  uint8_t* buffer_;
  uint8_t* current_payload_;
  uint32_t current_word_id_;

};

#endif /* sbnddaq_datatypes_Overlays_PennMilliSlice_hh */
