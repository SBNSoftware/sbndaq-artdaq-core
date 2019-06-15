#include "sbndaq-artdaq-core/Overlays/SBND/PTB_content.h"

#include <cstddef>

/*

std::set<unsigned short> ptb::content::word::ch_status_t::beam_channels() const {

  std::set<unsigned short> trs;

  beam_size_t mask = 0x1 ;

  for ( size_t i = 0 ; i < 9 ; ++i ) {
    if ( beam & (mask << i) ) {
      trs.insert( i ) ;
    }
  }

  return trs ;

}

std::set<unsigned short> ptb::content::word::ch_status_t::crt_channels() const {

  std::set<unsigned short> trs;

  crt_size_t mask = 0x1 ;

  for ( size_t i = 0 ; i < 32 ; ++i ) {
    if ( crt & (mask << i) ) {
      trs.insert( i ) ;
    }
  }

  return trs ;

}

std::set<unsigned short> ptb::content::word::ch_status_t::pds_channels() const {

  std::set<unsigned short> trs;

  pds_size_t mask = 0x1 ;
  pds_size_t pds_word = get_pds() ;

  for ( size_t i = 0 ; i < 24 ; ++i ) {
    if ( pds_word & (mask << i) ) {
      trs.insert( i ) ;
    }
  }

  return trs ;

}

*/

uint64_t ptb::content::word::word_t::timestamp_ns() const {

  uint64_t ts_ns = timestamp * 20; //50MHz clock ->  20ns/tick

  return ts_ns ;

}

std::set<unsigned short> ptb::content::word::trigger_t::Triggers( size_t max_bit ) const {

  std::set<unsigned short> trs;

  mask_size_t mask = 0x1 ;

  for ( size_t i = 0 ; i < max_bit ; ++i ) {
    if ( trigger_word & (mask << i) ) {
      trs.insert( i ) ;
    }
  }

  return trs ;

}
