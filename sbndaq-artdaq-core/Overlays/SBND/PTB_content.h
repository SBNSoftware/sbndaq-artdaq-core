/*
 * content.h
 *
 *  Created on: Oct 24, 2018
 *      Author: nbarros
 *      Modified and maintained by: D. Rivera for SBND
 */

#ifndef SBNDAQ_OVERLAYS_SBND_PTB_CONTENT_H_
#define SBNDAQ_OVERLAYS_SBND_PTB_CONTENT_H_

#include <set>
#include <cstdint>

namespace ptb {

  namespace content {

    using std::size_t ;

    // NFB: Note that order denotes bit order in the word:
    //      Upper fields are in the lsb and lower field is in the msb

    /// Internal buffer definition
    ///

    //static uint8_t format_version = 0x2;

    typedef struct buffer_t {
        int       handle;
        size_t    len;
    } buffer_t;

    ///
    /// -- TCP header
    ///
    typedef struct tcp_header_t {
        typedef uint16_t pkt_size_t;
        typedef uint8_t   seq_size_t;
        typedef uint8_t   ver_size_t;
        uint16_t packet_size    : 16; // Size of the data content in bytes
        uint8_t  sequence_id    : 8; // packet order...rotates every 256
        uint8_t  format_version : 8;

        static size_t const size_bytes = sizeof(uint32_t);
        static size_t const n_bits_size                 = 16;
        static size_t const n_bits_sequence_id  = 8;
        static size_t const n_bits_version          = 8;

    } tcp_header_t;

    // NFB: Careful with unions. Setting one member and then accessing another
    // is undefined behavior in C++.
    // However, I have tested that they work on gcc on the PTB
    typedef union tcp_header {
        tcp_header_t word;
        uint32_t         value;
    } tcp_header;

    namespace word {


      enum word_type {t_fback=0x0,t_lt=0x1,t_gt=0x2, t_ch=0x3,t_chksum=0x4,t_ts=0x7};



      ///
      /// -- payload
      ///

      // -- The body of a word is composed of 12 bytes
      //typedef uint16_t trigger_code_t;
      typedef uint64_t trigger_code_t;


      ///
      /// N. Barros - May 7, 2018 : Changed the structure of the parsing
      ///
      /// Now all structures cast into 16 bytes, but declare different
      /// bit fields to interpret each frame in the context of its respective type
      ///
      /// Also, to move into a format where the full timestamp is stored, the full timestamp
      /// is now placed in the 64 lsb, instead of the msb. Does complies with the memory alignment
      /// and simplifies the parsing of the structures
      ///

      typedef struct word_t {

        static size_t const n_bits_timestamp  = 64;
        static size_t const n_bits_payload    = 61;
        static size_t const n_bits_type       = 3;

        typedef uint64_t ts_size_t;
        typedef uint64_t pad_size_t;
        typedef uint64_t  word_type_t;

        ts_size_t timestamp;
        pad_size_t payload    : n_bits_payload  ;
        word_type_t word_type : n_bits_type ;

        //static size_t const size_bytes = sizeof( word_t );
        static size_t const size_bytes = 4*sizeof( uint32_t );
        static size_t const size_u32 = size_bytes/sizeof(uint32_t);

        // -- D.R. : allow for conversion of all ts to 1ns ticks 
        uint64_t timestamp_ns() const;

      } word_t;




      /// -- Several different structures that can be used to reinterpret the payload depending on
      /// the word type. All these structures map into the full 16 bytes of the CTB words
      ///
      typedef struct feedback_t {

          static size_t const n_bits_timestamp  = 64 ;
          static size_t const n_bits_code       = 8  ;
          static size_t const n_bits_source     = 8  ;
          static size_t const n_bits_payload    = 45 ;
          static size_t const n_bits_type       = word_t::n_bits_type ;

          typedef uint64_t  ts_size_t;
          typedef uint64_t  code_size_t;
          typedef uint64_t  source_size_t;
          typedef uint64_t  payload_size_t;
          typedef uint64_t  wtype_size_t;

          ts_size_t       timestamp ;
          code_size_t     code      : n_bits_code ;
          source_size_t   source    : n_bits_source ;
          payload_size_t  payload   : n_bits_payload;
          wtype_size_t    word_type : n_bits_type ;

          //static size_t const size_bytes = sizeof( feedback_t ) ;
          static size_t const size_bytes = 2*sizeof( uint64_t ) ;
          static size_t const size_u32 = size_bytes/sizeof(uint32_t);


        } feedback_t;


      //D.R. 10/24/2018 - Channel status word definition for SBND
      //v1.0 - timestamp only goes to 58 bits as of Oct 24, 2018 2PM for a 50MHz clock in time since the Epoch;
      //       won't rollover until the year 3431 for a 61-bit timestamp

      typedef struct ch_status_t {

          static size_t const n_bits_timestamp  = 61 ;
          static size_t const n_bits_beam       =  3 ;
          static size_t const n_bits_crt        = 14 ;
          static size_t const n_bits_pds        = 10 ;
          static size_t const n_bits_mtca       =  6 ;
          static size_t const n_bits_nim        =  6 ;
          static size_t const n_bits_auxpds     = 25 ;
          static size_t const n_bits_type       = word_t::n_bits_type ;

          typedef uint64_t ts_size_t;
          typedef uint64_t beam_size_t;
          typedef uint64_t crt_size_t;
          typedef uint64_t pds_size_t;
          typedef uint64_t mtca_size_t;
          typedef uint64_t nim_size_t;
          typedef uint64_t auxpds_size_t;
          typedef uint64_t wtype_size_t;

           ////////bits 0-63//////////////
           ts_size_t     timestamp  : n_bits_timestamp ;
           beam_size_t   beam       : n_bits_beam ;
           ////////bits 64-125////////////
           crt_size_t    crt        : n_bits_crt ;
           pds_size_t    pds        : n_bits_pds ;
           mtca_size_t   mtca       : n_bits_mtca ;
           nim_size_t    nim        : n_bits_nim ;
           auxpds_size_t auxpds     : n_bits_auxpds ; //leftovers are for the v2495
           wtype_size_t  word_type  : n_bits_type ;

           //static size_t const size_bytes = sizeof( ch_status_t );
           static size_t const size_bytes = 2*sizeof( uint64_t );
           static size_t const size_u32 = size_bytes/sizeof(uint32_t);

           // aux_functions Not sure why they are needed
           /*
           uint8_t  get_beam()   {return (beam   & 0x7)      ;}
           uint16_t get_crt()    {return (crt    & 0x3FFF)   ;}
           uint16_t get_pds()    {return (pds    & 0x3FF)    ;}
           uint8_t  get_mtca()   {return (mtca   & 0x3F)     ;}
           uint8_t  get_nim()    {return (mtca   & 0x3F)     ;}
           uint32_t get_auxpds() {return (auxpds & 0x1FFFFFF);} //25{1'b1}
           */
           
           bool get_state_crt(const uint16_t channel) {
             return (    (crt & (0x1 << channel)) != 0x0);
           }
           bool get_state_pds(const uint16_t channel) {
             return (    (pds & (0x1 << channel)) != 0x0);
           }
           bool get_state_beam(const uint16_t channel) {
             return (   (beam & (0x1 << channel)) != 0x0);
           }
           bool get_state_mtca(const uint16_t channel) {
             return (   (mtca & (0x1 << channel)) != 0x0);
           }
           bool get_state_nim(const uint16_t channel) {
             return (    (nim & (0x1 << channel)) != 0x0);
           }
           bool get_state_aux(const uint16_t channel) {
             return ( (auxpds & (0x1 << channel)) != 0x0);
           }

       } ch_status_t;


       typedef struct timestamp_t {

           static size_t const n_bits_timestamp = 64;
           static size_t const n_bits_unused    = 61;
           static size_t const n_bits_type      = 3;

           typedef uint64_t ts_size_t;
           typedef uint64_t pad_size_t;
           typedef uint64_t wtype_size_t;

           ts_size_t    timestamp;
           pad_size_t   padding   : n_bits_unused ;
           wtype_size_t word_type : n_bits_type ;

           //static size_t const size_bytes = sizeof( timestamp_t );
           static size_t const size_bytes = 2*sizeof( uint64_t );
           static size_t const size_u32 = size_bytes/sizeof(uint32_t);

       } timestamp_t;


       typedef struct trigger_t {

           static size_t const n_bits_timestamp = 64;
           static size_t const n_bits_tmask     = 61;
           static size_t const n_bits_type      = word_t::n_bits_type ;

           typedef uint64_t ts_size_t;
           typedef uint64_t mask_size_t;
           typedef uint64_t wtype_size_t;

           ts_size_t timestamp;
           mask_size_t  trigger_word : n_bits_timestamp ;
           wtype_size_t word_type    : n_bits_type ;

           //static size_t const size_bytes = sizeof( trigger_t );
           static size_t const size_bytes = 2*sizeof( uint64_t );
           static size_t const size_u32 = size_bytes/sizeof(uint32_t);

         bool IsHLT() const { return word_type == word_type::t_gt ; }
         bool IsLLT() const { return word_type == word_type::t_lt ; }
         bool IsTrigger( const unsigned int i ) const  { return trigger_word & ( 0x1 << i ); }
         std::set<unsigned short> Triggers( size_t max_bit = n_bits_tmask ) const ;

       } trigger_t;


       typedef union word{
           word_t frame;
           feedback_t feedback ;
           ch_status_t ch_status ;
           timestamp_t timestamp ;
           trigger_t trigger ;
           uint8_t *get_bytes() {return reinterpret_cast<uint8_t*>(&frame);}
       } word;

    } // -- namespace word
  } // -- namespace content
} // -- namespace ptb




#endif /* SBNDAQ_OVERLAYS_SBND_PTB_CONTENT_H_ */
