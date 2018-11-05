#ifndef sbnddaq_datatypes_Overlays_PennMicroSlice_hh
#define sbnddaq_datatypes_Overlays_PennMicroSlice_hh

#include <cstdint>
//#include <stddef.h>
#include <string>
//#define PENN_OLD_STRUCTS

namespace sbnddaq {
  class PennMicroSlice;
}

class sbnddaq::PennMicroSlice {

public:

#ifdef PENN_OLD_STRUCTS
  struct Header {

    typedef uint32_t data_t;
    
    typedef uint8_t  format_version_t;
    typedef uint8_t  sequence_id_t;
    typedef uint16_t block_size_t;

    // JCF, Jul-28-15

    // The order of these variables have been reversed to reflect that
    // the block size takes up the two least significant bytes, the
    // sequence ID the second most significant byte, and the format
    // version the most significant byte, of the four-byte microslice header

    block_size_t     block_size     : 16;
    sequence_id_t    sequence_id    : 8;
    format_version_t format_version : 8;



    static size_t const size_words = sizeof(data_t);

    //static constexpr size_t raw_header_words = 1;
    //data_t raw_header_data[raw_header_words];
  };

  // Nanoslice
  struct Payload_Header {

    typedef uint32_t data_t;

    typedef uint8_t  data_packet_type_t;
    typedef uint32_t short_nova_timestamp_t;

    // JCF, Jul-28-15

    // The order of the data packet type and the timestamp have been
    // swapped to reflect that it's the MOST significant three bits in
    // the payload header which contain the type. I've also added a
    // 1-bit pad to reflect that the least significant bit is unused.

    uint8_t padding : 2;
    short_nova_timestamp_t short_nova_timestamp : 27;
    data_packet_type_t     data_packet_type     : 3;

    static size_t const size_words = sizeof(data_t);
  };

  struct Warning_Header {
    typedef uint32_t data_t;
    typedef uint16_t data_size_t;

    typedef uint8_t  warning_type_t;
    typedef uint8_t  data_packet_type_t;
    typedef uint32_t short_nova_timestamp_t;

    // The order of the data packet type and the timestamp have been
    // swapped to reflect that it's the MOST significant three bits in
    // the payload header which contain the type. I've also added a
    // 1-bit pad to reflect that the least significant bit is unused.

    uint32_t padding : 24;
    warning_type_t warning_type : 5;
    data_packet_type_t     data_packet_type     : 3;

    static size_t const size_words = sizeof(data_t);
    static data_size_t const num_bits_padding     = 24;
    static data_size_t const num_bits_warning  = 5;
    static data_size_t const num_bits_packet_type = 3;
  };

#else
  /// Header of ethernet packet
  struct Header {

  typedef uint32_t data_t;
  typedef uint16_t data_size_t;

  typedef uint8_t  format_version_t;
  typedef uint8_t  sequence_id_t;
  typedef uint16_t block_size_t;

  block_size_t     block_size     : 16;
  sequence_id_t    sequence_id    : 8;
  format_version_t format_version : 8;

  static size_t const size_words = sizeof(data_t);

  static data_size_t const num_bits_size        = 16;
  static data_size_t const num_bits_sequence_id = 8;
  static data_size_t const num_bits_format      = 8;
};

/// Internal warning word
struct Warning_Word {
  typedef uint32_t data_t;
  typedef uint16_t data_size_t;

  typedef uint8_t  warning_type_t;
  typedef uint8_t  data_packet_type_t;

  uint32_t padding                        : 24;
  warning_type_t warning_type             : 5;
  data_packet_type_t data_packet_type     : 3;

  static size_t const size_words = sizeof(data_t);
  static data_size_t const num_bits_padding     = 24;
  static data_size_t const num_bits_warning     = 5;
  static data_size_t const num_bits_packet_type = 3;
};

/// Frame header : Common header to everything except warning words
struct Payload_Header {
  typedef uint32_t data_t;
  typedef uint16_t data_size_t;

  typedef uint8_t  data_packet_type_t;
  typedef uint32_t short_nova_timestamp_t;

  // The order of the data packet type and the timestamp have been
  // swapped to reflect that it's the MOST significant three bits in
  // the payload header which contain the type. I've also added a
  // 2-bit pad to reflect that the 2 least significant bits are unused.

  uint8_t padding                             : 2;
  short_nova_timestamp_t short_nova_timestamp : 27;
  data_packet_type_t     data_packet_type     : 3;

  static size_t const size_bytes = sizeof(data_t);
  static size_t const size_u32 = sizeof(data_t)/sizeof(uint32_t);

  static data_size_t const num_bits_padding       = 2;
  static data_size_t const num_bits_short_tstamp  = 27;
  static data_size_t const num_bits_packet_type   = 3;

  // This function converts the TS rollover into a full TS
  // it takes the payload of the timestamp word that came after this word
  // (the microslice border)
  // and calculates the offset
  // to use the full timestamp received before use the other method (pre)
  uint64_t get_full_timestamp_pre(uint64_t ts_ref) {
    if ((ts_ref & 0x7FFFFFF) == short_nova_timestamp) {
      // they are equal. This word is right on the border of the microslice
      return ts_ref;
    } else if ((ts_ref & 0x7FFFFFF) > short_nova_timestamp) {
      // there was no bit rollover in between
      return ts_ref - ((ts_ref & 0x7FFFFFF) - short_nova_timestamp);
    } else {
      // it rolled over.
      // Be sure of the values being set
      return ts_ref - ((ts_ref & 0x7FFFFFF) + (0x7FFFFFF - short_nova_timestamp));
    }
  }
  // Does the same as the previous but with the timestamp that came before
  // They should be equivalent but need to check to confirm
  uint64_t get_full_timestamp_post(uint64_t ts_ref) {
    if ((ts_ref & 0x7FFFFFF) == short_nova_timestamp) {
      // they are equal. This word is right on the border of the microslice
      return ts_ref;
    } else if ((ts_ref & 0x7FFFFFF) > short_nova_timestamp) {
      // it rolled over. Has to sum the short and the difference
      // that takes for the reference to roll over
      return ts_ref + (0x7FFFFFF - ((ts_ref & 0x7FFFFFF))) + short_nova_timestamp;
    } else {
      // it didn't roll over. Just add the difference of the rollovers
      return ts_ref + (short_nova_timestamp - (ts_ref & 0x7FFFFFF));
    }
  }


};

/// Counter payload description
struct Payload_Counter {
  typedef uint64_t counter_set_t;
  typedef uint16_t data_size_t;
  // -- Must be careful to follow the right order
  // from lsb to msb it is
  // -- TSU mappings
  counter_set_t tsu_wu     : 10;
  counter_set_t tsu_el     : 10;
  counter_set_t tsu_extra  :  4;
  counter_set_t tsu_nu     :  6;
  counter_set_t tsu_sl     :  6;
  counter_set_t tsu_nl     :  6;
  counter_set_t tsu_su     :  6;
  // -- BSU mappings
  counter_set_t bsu_rm     : 16;//end of first counter_set_t==uint64_t
  counter_set_t bsu_cu     : 10;
  //FIXME: The panels should be remapped so that bsu_cl would be all together
  counter_set_t bsu_cl1    : 6;
  counter_set_t bsu_extra  : 1;
  counter_set_t bsu_cl2    : 7;
  counter_set_t bsu_rl     : 10;
  // Just ignore the rest of the word
  counter_set_t padding    : 30;

  static data_size_t const num_bits_tsu_wu    = 10;
  static data_size_t const num_bits_tsu_el    = 10;
  static data_size_t const num_bits_tsu_extra =  4;
  static data_size_t const num_bits_tsu_nu    =  6;
  static data_size_t const num_bits_tsu_sl    =  6;
  static data_size_t const num_bits_tsu_nl    =  6;
  static data_size_t const num_bits_tsu_su    =  6;
  static data_size_t const num_bits_bsu_rm    = 16;
  static data_size_t const num_bits_bsu_cu    = 10;
  static data_size_t const num_bits_bsu_cl1   = 6;
  static data_size_t const num_bits_bsu_extra = 1; // -- unused channel (32)
  static data_size_t const num_bits_bsu_cl2   = 7;
  static data_size_t const num_bits_bsu_rl    = 10;
  static data_size_t const num_bits_padding   = 30;
  static data_size_t const num_bits_bsu_cl    = 13;

  // position boundaries for the different counter types
  // it also works as a sort of enumerator for the different counter types
  // so that they can be uniquely identified
  // the value is the first element outside the type
  static data_size_t const counter_type_tsu_wu    = 10;
  static data_size_t const counter_type_tsu_el    = 20;
  static data_size_t const counter_type_tsu_extra = 24;
  static data_size_t const counter_type_tsu_nu    = 30;
  static data_size_t const counter_type_tsu_sl    = 36;
  static data_size_t const counter_type_tsu_nl    = 42;
  static data_size_t const counter_type_tsu_su    = 48;
  static data_size_t const counter_type_bsu_rm    = 64;
  static data_size_t const counter_type_bsu_cu    = 74;
  static data_size_t const counter_type_bsu_cl1   = 80;
  static data_size_t const counter_type_bsu_extra = 81; // -- unused channel (32)
  static data_size_t const counter_type_bsu_cl2   = 88;
  static data_size_t const counter_type_bsu_rl    = 98;
  static data_size_t const counter_type_padding   = 128;
  // special types that are only used in the enum
  static data_size_t const counter_type_unknown   = 129;
  static data_size_t const counter_type_bsu_cl    = 130;

  static size_t const size_bytes = 2*sizeof(uint64_t);
  static size_t const size_u32 = size_bytes/sizeof(uint32_t);

  // The size that arrives from the PTB
  static size_t const size_words_ptb_u32 = 3;
  static size_t const size_words_ptb_bytes = size_words_ptb_u32*sizeof(uint32_t);
  // The offset to grab the data
  static size_t const ptb_offset_u32 = 0;
  static size_t const ptb_offset_bytes = ptb_offset_u32*sizeof(uint32_t);

  // The payload position offset from the top of the frame (header + discard)
  static size_t const payload_offset_u32 = 1+ptb_offset_u32;
  static size_t const payload_offset_bytes = payload_offset_u32*sizeof(uint32_t);

  counter_set_t get_bsu_cl() {return ((bsu_cl2 << 10) | (bsu_cl1));};

  bool get_tsu_wu(uint32_t idx) {
    return ((tsu_wu & (1 << idx)) != 0x0);
  }
  bool get_tsu_el(uint32_t idx) {
    return ((tsu_el & (1 << idx)) != 0x0);
  }
  bool get_tsu_extra(uint32_t idx) {
    return ((tsu_extra & (1 << idx)) != 0x0);
  }
  bool get_tsu_nu(uint32_t idx) {
    return ((tsu_nu & (1 << idx)) != 0x0);
  }
  bool get_tsu_sl(uint32_t idx) {
    return ((tsu_sl & (1 << idx)) != 0x0);
  }
  bool get_tsu_nl(uint32_t idx) {
    return ((tsu_nl & (1 << idx)) != 0x0);
  }
  bool get_tsu_su(uint32_t idx) {
    return ((tsu_su & (1 << idx)) != 0x0);
  }
  // BSU getters
  bool get_bsu_rm(uint32_t idx) {
    return ((bsu_rm & (1 << idx)) != 0x0);
  }
  bool get_bsu_cu(uint32_t idx) {
    return ((bsu_cu & (1 << idx)) != 0x0);
  }
  // The CL is a bit special due to the crazy mapping
  bool get_bsu_cl(uint32_t idx) {
    if (idx <  num_bits_bsu_cl1) {
      return ((bsu_cl1 & (1 << idx)) != 0x0);
    } else {
      return ((bsu_cl2 & (1 << (idx-num_bits_bsu_cl1))) != 0x0);
    }
  }
  bool get_bsu_rl(uint32_t idx) {
    return ((bsu_rl & (1 << idx)) != 0x0);
  }

  bool get_counter_status(uint32_t idx) const {
    // This was tested to return the right bits in the right order in a standalone test
    // However, it is up to the user to know what type this bit corresponds to
    return (((reinterpret_cast<const uint8_t*>(this)[idx/8]) & (1 << (idx%8))) != 0x0);
  }

  static data_size_t get_counter_type(uint32_t bit_idx) {
    // NFB: By construction bit_idx cannot be negative
    //    if (bit_idx < 0) return counter_type_unknown; else
    if (bit_idx < counter_type_tsu_wu) return counter_type_tsu_wu;
    else if (bit_idx < counter_type_tsu_el) return counter_type_tsu_el;
    else if (bit_idx < counter_type_tsu_extra) return counter_type_tsu_extra;
    else if (bit_idx < counter_type_tsu_nu) return counter_type_tsu_nu;
    else if (bit_idx < counter_type_tsu_sl) return counter_type_tsu_sl;
    else if (bit_idx < counter_type_tsu_nl) return counter_type_tsu_nl;
    else if (bit_idx < counter_type_tsu_su) return counter_type_tsu_su;
    else if (bit_idx < counter_type_bsu_rm) return counter_type_bsu_rm;
    else if (bit_idx < counter_type_bsu_cu) return counter_type_bsu_cu;
    else if (bit_idx == (counter_type_bsu_extra-1)) return counter_type_bsu_extra;
    else if (bit_idx < counter_type_bsu_cl2) return counter_type_bsu_cl;
    else if (bit_idx < counter_type_bsu_rl) return counter_type_bsu_rl;
    else if (bit_idx < counter_type_padding) return counter_type_padding;
    else return counter_type_unknown;
  }
  // returns the index of the counter within its own group
  // for instance get_counter_type_pos(10) should return 0 as this is
  // the first counter in the tsu_el group
static uint32_t get_counter_type_pos(uint32_t bit_idx) {
  if (bit_idx < counter_type_tsu_wu) return bit_idx;
  else if (bit_idx < counter_type_tsu_el) return (bit_idx - counter_type_tsu_wu);
  else if (bit_idx < counter_type_tsu_extra) return (bit_idx - counter_type_tsu_el);
  else if (bit_idx < counter_type_tsu_nu) return (bit_idx - counter_type_tsu_extra);
  else if (bit_idx < counter_type_tsu_sl) return (bit_idx - counter_type_tsu_nu);
  else if (bit_idx < counter_type_tsu_nl) return (bit_idx - counter_type_tsu_sl);
  else if (bit_idx < counter_type_tsu_su) return (bit_idx - counter_type_tsu_nl);
  else if (bit_idx < counter_type_bsu_rm) return (bit_idx - counter_type_tsu_su);
  else if (bit_idx < counter_type_bsu_cu) return (bit_idx - counter_type_bsu_rm);
  else if (bit_idx == (counter_type_bsu_extra-1)) return 0;
  else if (bit_idx < counter_type_bsu_cl1) return (bit_idx - counter_type_bsu_cu);
  // -- offset the extra position that is unused in between
  else if (bit_idx < counter_type_bsu_cl2) return (bit_idx - counter_type_bsu_cu -1);
  else if (bit_idx < counter_type_bsu_rl) return (bit_idx - counter_type_bsu_cl2);
  else if (bit_idx < counter_type_padding) return (bit_idx - counter_type_bsu_rl);
  else return counter_type_unknown;

}

};

/// Trigger description
struct Payload_Trigger {
  typedef uint32_t trigger_type_t;
  typedef uint16_t data_size_t;

  // The 16 lsb are padding. No information is passed there
  trigger_type_t padding_low : 8;

  // This is to be remapped so that calib words can be OR-ed with
  // calibration words
  // [8 : t_type][4 : muon_type][4 : calib type]
  trigger_type_t trigger_id_ext  : 4;
  trigger_type_t trigger_id_calib: 4; // which of the calibration channels
  trigger_type_t trigger_id_muon : 8; // which of the muon triggers
  trigger_type_t trigger_type    : 5; // the 5 msb are the trigger type
  trigger_type_t padding_high    : 3; // this makes the information byte aligned

  static data_size_t const num_bits_padding_low     = 8;
  static data_size_t const num_bits_trigger_id_ext  = 4;
  static data_size_t const num_bits_trigger_id_calib= 4;
  static data_size_t const num_bits_trigger_id_muon = 8;
  static data_size_t const num_bits_trigger_type    = 5;
  static data_size_t const num_bits_padding_high    = 3;

  // The logic below this point is a bit different since now multiple
  // triggers can be ID-ed in the same word
  // Better to ask for specific types


  // ID the trigger types for figure reference
  // The logic is flawed. Need to look at this otherwise
  /** Old map
  static trigger_type_t const calibration = 0x00; //00000
  static trigger_type_t const muon        = 0x10; //10000
  static trigger_type_t const ssp         = 0x08; //01000
  // -- This should probably be split into RCE and then RCE types
  static trigger_type_t const rce_1       = 0x01; //00001
  static trigger_type_t const rce_2       = 0x02; //00010
  static trigger_type_t const rce_12      = 0x03; //00011
  static trigger_type_t const rce_3       = 0x04; //00100
  static trigger_type_t const rce_13      = 0x05; //00101
  static trigger_type_t const rce_23      = 0x06; //00110
  static trigger_type_t const rce_123     = 0x07; //00111
   **/
  static trigger_type_t const muon        = 0x10; //10000
  static trigger_type_t const external    = 0x08; //01000
  static trigger_type_t const calibration = 0x04; //00100

  // -- This should probably be split into RCE and then RCE types
  // External trigger types
  static trigger_type_t const I01_08      = 0x8; //1000
  static trigger_type_t const ssp         = 0x8; //1000
  static trigger_type_t const I09         = 0x4; //0100
  static trigger_type_t const I10         = 0x2; //0010
  static trigger_type_t const I12         = 0x1; //0001
  // NOTE: There is no I11. It is a adead channel.

  // C1 : 1000 : 0x8
  // C2 : 0100 : 0x4
  // C3 : 0010 : 0x2
  // C4 : 0001 : 0x1
  static trigger_type_t const C1 = 0x8;
  static trigger_type_t const C2 = 0x4;
  static trigger_type_t const C3 = 0x2;
  static trigger_type_t const C4 = 0x1;

  // TA : BSU RM/CL : 10000000
  // TB : TSU SL/NU : 01000000
  // TC : TSU SU/NL : 00100000
  // TD : TSU EL/WU : 00010000
  // TE : ????????? : 00001000 <-- Not implemented in FW
  // TF : ????????? : 00000100 <-- Not implemented in FW
  // TG : ????????? : 00000010 <-- Not implemented in FW
  // TH : ????????? : 00000001 <-- Not implemented in FW
  static trigger_type_t const TA = 0x80;
  static trigger_type_t const TB = 0x40;
  static trigger_type_t const TC = 0x20;
  static trigger_type_t const TD = 0x10;
  static trigger_type_t const TE = 0x08;
  static trigger_type_t const TF = 0x04;
  static trigger_type_t const TG = 0x02;
  static trigger_type_t const TH = 0x01;

  static trigger_type_t const bsu_rm_cl = 0x80;
  static trigger_type_t const tsu_sl_nu = 0x40;
  static trigger_type_t const tsu_su_nl = 0x20;
  static trigger_type_t const tsu_el_wu = 0x10;

  static size_t const size_bytes = sizeof(uint32_t);
  static size_t const size_u32 = size_bytes/sizeof(uint32_t);

  // The number of u32 that have to offset from the PTB packet to grab the data that matters
  // In this case the two lsInts are empty and can be offset
  static size_t const ptb_offset_u32 = 2;
  static size_t const ptb_offset_bytes = ptb_offset_u32*sizeof(uint32_t);

  // The payload position offset from the top of the frame (header + discard)
  static size_t const payload_offset_u32 = 1+ptb_offset_u32;
  static size_t const payload_offset_bytes = payload_offset_u32*sizeof(uint32_t);



  ///Bunch of auxiliary functions to help parse the word
  ///
  bool has_muon_trigger() {
    return ((trigger_type & muon) != 0x0);
  }
  bool has_external_trigger() {
    return ((trigger_type & external) != 0x0);
  }
  bool has_calibration() {
    return ((trigger_type & calibration) != 0x0);
  }

  // -- External trigger types

  bool has_external_trigger(trigger_type_t t) {
    return ((trigger_id_ext & t) != 0x0);
  }

  bool has_ssp_trigger() {
    return ((trigger_id_ext & ssp) != 0x0);
  }
  bool has_I09() {
    return ((trigger_id_ext & I09) != 0x0);
  }
  bool has_I10() {
    return ((trigger_id_ext & I10) != 0x0);
  }
  bool has_I12() {
    return ((trigger_id_ext & I12) != 0x0);
  }

  // calibration trigger types
  /// Test for the different calibration types
  ///

  bool has_calibration(trigger_type_t t) {
    return ((trigger_id_calib & t) != 0x0);
  }

  bool has_C1() {
    return ((trigger_id_calib & C1) != 0);
  }
  bool has_C2() {
    return ((trigger_id_calib & C2) != 0);
  }
  bool has_C3() {
    return ((trigger_id_calib & C3) != 0);
  }
  bool has_C4() {
    return ((trigger_id_calib & C4) != 0);
  }

  /// Test the different muon trigger types
  ///
  bool has_muon_trigger(trigger_type_t t) {
    return ((trigger_id_muon & t) != 0x0);
  }
  ///
  bool has_muon_TA() {
    return ((trigger_id_muon & TA) != 0);
  }
  bool has_muon_TB() {
    return ((trigger_id_muon & TB) != 0);
  }
  bool has_muon_TC() {
    return ((trigger_id_muon & TC) != 0);
  }
  bool has_muon_TD() {
    return ((trigger_id_muon & TD) != 0);
  }
  bool has_muon_TE() {
    return ((trigger_id_muon & TE) != 0);
  }
  bool has_muon_TF() {
    return ((trigger_id_muon & TF) != 0);
  }
  bool has_muon_TG() {
    return ((trigger_id_muon & TG) != 0);
  }
  bool has_muon_TH() {
    return ((trigger_id_muon & TH) != 0);
  }

  /// Test the different muon trigger types
  ///
  bool has_muon_bsu_rm_cl() {
    return ((trigger_id_muon & bsu_rm_cl) != 0);
  }
  bool has_muon_tsu_sl_nu() {
    return ((trigger_id_muon & tsu_sl_nu) != 0);
  }
  bool has_muon_tsu_su_nl() {
    return ((trigger_id_muon & tsu_su_nl) != 0);
  }
  bool has_muon_tsu_el_wu() {
    return ((trigger_id_muon & tsu_el_wu) != 0);
  }


  // Add a function that can be used to parse the trigger payload
  //FIXME: This should be considered obsolete and removed
  // in a near future

  static std::string getTriggerTypeName(trigger_type_t trigger_type) {
    switch (trigger_type) {
      case calibration:
        return "calibration";
        break;
      case external:
        return "external";
        break;
      case muon :
        return "muon";
        break;
      default:
        return "unknown";
        break;
    }
    return "";
  }
};

struct Payload_Timestamp {
  typedef uint64_t timestamp_t;
  typedef uint16_t data_size_t;
  timestamp_t nova_timestamp : 64;

  static data_size_t const num_bits_timestamp = 64;
  static size_t const size_bytes = sizeof(uint64_t);
  static size_t const size_u32 = size_bytes/sizeof(uint32_t);
  // drop 1 int
  static size_t const ptb_offset_u32 = 1;
  static size_t const ptb_offset_bytes = ptb_offset_u32*sizeof(uint32_t);
  // The payload position offset from the top of the frame (header + discard)

  static size_t const payload_offset_u32 = 1+ptb_offset_u32;
  static size_t const payload_offset_bytes = payload_offset_u32*sizeof(uint32_t);

};

#endif /*PENN_OLD_STRUCTS*/

  typedef Header::block_size_t microslice_size_t;

  //the size of the payloads (neglecting the Payload_Header)

  static microslice_size_t const payload_size_counter   = 4 * sizeof(uint32_t); //128-bit payload
  static microslice_size_t const payload_size_trigger   = 1 * sizeof(uint32_t); //32-bit payload
  static microslice_size_t const payload_size_timestamp = 2 * sizeof(uint32_t); //64-bit payload
  static microslice_size_t const payload_size_warning   = 0 * sizeof(uint32_t); //0-bit payload (just header)
  static microslice_size_t const payload_size_checksum  = 0 * sizeof(uint32_t); //0-bit payload (just header)


  //The types of data words
  static const Payload_Header::data_packet_type_t DataTypeWarning   = 0x0; //0b000
  static const Payload_Header::data_packet_type_t DataTypeCounter   = 0x1; //0b001
  static const Payload_Header::data_packet_type_t DataTypeTrigger   = 0x2; //0b010
  static const Payload_Header::data_packet_type_t DataTypeChecksum  = 0x4; //0b100
  static const Payload_Header::data_packet_type_t DataTypeTimestamp = 0x7; //0b111


  //The types of data words
  static const Warning_Word::warning_type_t WarnUnknownDataType   = 0x02;  //0b00010
  static const Warning_Word::warning_type_t WarnTimeout           = 0x04;  //0b00100
  static const Warning_Word::warning_type_t WarnFIFOHalfFull      = 0x08;  //0b01000
  static const Warning_Word::warning_type_t WarnFIFOFull          = 0x10; //0b10000


  // This constructor accepts a memory buffer that contains an existing
  // microSlice and allows the the data inside it to be accessed
  PennMicroSlice(uint8_t* address);

  // Get the contents of a payload
  // Why is everything worked in bytes and not 32 bit units?
  uint8_t* get_payload(uint32_t word_id, Payload_Header::data_packet_type_t& data_packet_type, Payload_Header::short_nova_timestamp_t& short_nova_timestamp, size_t& size, bool swap_payload_header_bytes, size_t override_uslice_size = 0) const;
  // Get the contents of the next payload
  //JPD -- get_payload is inefficient for user code use - it has to loop through (i) payloads to get the (ith) payload. This function increments the current_payload_ pointer to the next payload, and returns nullptr when off the end
  uint8_t* get_next_payload(uint32_t &word_id, Payload_Header::data_packet_type_t& data_packet_type, Payload_Header::short_nova_timestamp_t& short_nova_timestamp, size_t& size, bool swap_payload_header_bytes, size_t override_uslice_size = 0);

  // Returns the format version field from the header
  Header::format_version_t format_version() const;

  // Returns the sequence ID field from the header
  Header::sequence_id_t sequence_id() const;

  // Returns the block size field from the header
  Header::block_size_t block_size() const;

  // Returns the size of the PennMicroSlice
  sbnddaq::PennMicroSlice::microslice_size_t size() const;

  // Returns the number of samples in the microslice

  typedef uint32_t sample_count_t;

  sample_count_t sampleCount(sample_count_t &n_counter_words, sample_count_t &n_trigger_words, 
			     sample_count_t &n_timestamp_words,
			     sample_count_t &n_selftest_words, sample_count_t &n_checksum_words,
			     bool swap_payload_header_bytes, size_t override_uslice_size = 0) const;

  // Returns a pointer to the first payload_header in data that has a
  // time after boundary_time (returns 0 if that doesn't exist) also
  // sets remaining_size - the size of data that is after
  // boundary_time

  uint8_t* sampleTimeSplit(uint64_t boundary_time, size_t& remaining_size, bool swap_payload_header_bytes, 
			   size_t override_uslice_size = 0) const;

  // Returns a pointer to the first payload_header in data that has a
  // time after boundary_time (returns 0 if that doesn't exist) also
  // sets remaining_size - the size of data that is after
  // boundary_time and counts payloads before & after the time NOTE
  // this is prefered to calling sampleCount() and sampleTimeSplit()
  // separately, as it loops through the data exactly once (instead of
  // between once & exactly twice)

  uint8_t* sampleTimeSplitAndCount(uint64_t boundary_time, size_t& remaining_size,
				   sample_count_t &n_words_b, sample_count_t &n_counter_words_b, 
				   sample_count_t &n_trigger_words_b,
				   sample_count_t &n_timestamp_words_b, sample_count_t &n_selftest_words_b, 
				   sample_count_t &n_checksum_words_b,
				   sample_count_t &n_words_a, sample_count_t &n_counter_words_a, 
				   sample_count_t &n_trigger_words_a,
				   sample_count_t &n_timestamp_words_a, sample_count_t &n_selftest_words_a, 
				   sample_count_t &n_checksum_words_a,
				   bool swap_payload_header_bytes, 
				   size_t override_uslice_size = 0) const;

  // As sampleTimeSplitAndCount, but also gets information for a second time (overlap_time), in the same loop

  uint8_t* sampleTimeSplitAndCountTwice(uint64_t boundary_time, size_t& remaining_size,
					uint64_t overlap_time,  size_t& overlap_size,   uint8_t*& overlap_data_ptr,
					sample_count_t &n_words_b, sample_count_t &n_counter_words_b, 
					sample_count_t &n_trigger_words_b,
					sample_count_t &n_timestamp_words_b, sample_count_t &n_selftest_words_b, 
					sample_count_t &n_checksum_words_b,
					sample_count_t &n_words_a, sample_count_t &n_counter_words_a, 
					sample_count_t &n_trigger_words_a,
					sample_count_t &n_timestamp_words_a, sample_count_t &n_selftest_words_a, 
					sample_count_t &n_checksum_words_a,
					sample_count_t &n_words_o, sample_count_t &n_counter_words_o, 
					sample_count_t &n_trigger_words_o,
					sample_count_t &n_timestamp_words_o, sample_count_t &n_selftest_words_o, 
					sample_count_t &n_checksum_words_o,
					uint32_t &checksum,
					bool swap_payload_header_bytes, size_t override_uslice_size = 0) const;

  //Values used to handle the rollover.
  //The width of the millislice should be:
  // LESS than the ROLLOVER_LOW_VALUE
  // LESS than (max of a uint28_t - ROLLOVER_HIGH_VALUE)
  // NFB : Not sure I understand this logic
  static const uint32_t ROLLOVER_LOW_VALUE  = 1 << 13; //8192 ticks = 0.128ms
  //static const uint32_t ROLLOVER_HIGH_VALUE = 1 << 26;
  static const uint64_t ROLLOVER_HIGH_VALUE = (1 << 27) -1;



  static uint64_t getMask(int param){
	uint64_t mask=0;
	mask = (1 << param) - 1;//sets the mask to 0000...11111...11
	return mask;
  };

  uint32_t* raw() const;

protected:

  // returns a pointer to the header
  Header const* header_() const;

  // returns a pointer to the first sample word
  uint32_t const* data_() const;

  uint8_t* buffer_;
  uint8_t* current_payload_;
  uint32_t current_word_id_;
};

#endif /* sbnddaq_datatypes_Overlays_PennMicroSlice_hh */
