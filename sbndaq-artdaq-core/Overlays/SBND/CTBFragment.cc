#include "sbndaq-artdaq-core/Overlays/SBND/CTBFragment.hh""sbndaq-artdaq-core/Overlays/SBND/CTBFragment.hh"

#include "cetlib/exception.h"

namespace sbndaq {


  std::ostream & operator << (std::ostream & out, CTBFragment const & f ) {

    out << "Fragment size: " << f.artdaq_Fragment_.dataSizeBytes() << " -> " << f._n_words << " words" << std::endl ;

    for ( unsigned int i = 0; i < f.NWords() ; ++i ) {

      if ( f.Trigger(i) ) {
        out << "Trigger word " << std::hex << f.Trigger(i) -> word_type
            << ", payload: "  << f.Trigger(i) -> trigger_word
            << ", TS: " << f.TimeStamp(i) << std::dec << std::endl ;
      }
      else if ( f.ChStatus(i) ) {
	out << "Check Status word " << std::hex
            << " PDS " << f.ChStatus(i) -> get_pds()
            << ", CRT: " << f.ChStatus(i) -> get_crt()
	    << ", Beam: " << f.ChStatus(i) -> get_beam()
	    << ", TS: " << f.TimeStamp(i)
	    << std::dec << std::endl ;

      }
      else if ( f.Feedback(i) ) {
	out << "Feedback word  " << std::hex
	    << ", Padding: " << f.Feedback(i) -> padding
	    << ", Source: " << f.Feedback(i) -> source
	    << ", Code: " << f.Feedback(i) -> code
            << ", TS: " << f.TimeStamp(i) << std::dec << std::endl ;

      }
      else {
	out << "type: " << std::hex << f.Word(i) -> word_type
	    << ", payload: " << f.Word(i) -> payload
	    << ", TS: " << f.TimeStamp(i) << std::dec << std::endl ;
      }

    }


    return out ;
  }



  CTBFragment::CTBFragment( artdaq::Fragment const & f ) :
    artdaq_Fragment_( f ),
    _n_words( f.dataSizeBytes() % CTBFragment::WordSize() == 0 ? f.dataSizeBytes()/CTBFragment::WordSize() : 0 )

  { ; }


  //---------------------------------
  // Proper TS getter
  //---------------------------------

  uint64_t CTBFragment::TimeStamp( unsigned int i ) const {

    const ptb::content::word::word_t* w = Word(i) ;


    if ( w -> word_type == ptb::content::word::t_ch ) {
      // ch status words have 4 less bits in the MSB
      // the missing bits are recovered from the fragment TS
      return ( (artdaq_Fragment_.timestamp() & 0xF0000000) | CTBFragment::ChStatus( *w ) -> timestamp ) ;
    }

    return w -> timestamp ;
  }



  //--------------------------------
  // word getters
  //--------------------------------

  const ptb::content::word::word_t* CTBFragment::Word( unsigned int i ) const {

    if ( i >= NWords() ) return nullptr ;

    return reinterpret_cast<const ptb::content::word::word_t*>( artdaq_Fragment_.dataBeginBytes() + i * CTBFragment::WordSize() )  ;

  }


  const ptb::content::word::feedback_t* CTBFragment::Feedback( unsigned int i ) const {

    const ptb::content::word::word_t* w = Word( i )  ;

    if ( ! w )  return nullptr ;

    return CTBFragment::Feedback( *w ) ;

  }


  const ptb::content::word::ch_status_t* CTBFragment::ChStatus( unsigned int i ) const {

    const ptb::content::word::word_t* w = Word( i )  ;

    if ( ! w )  return nullptr ;

    return CTBFragment::ChStatus( *w ) ;

  }


  const ptb::content::word::timestamp_t* CTBFragment::TSWord( unsigned int i ) const {

    const ptb::content::word::word_t* w = Word( i )  ;

    if ( ! w )  return nullptr ;

    return CTBFragment::TSWord( *w ) ;

  }


  const ptb::content::word::trigger_t * CTBFragment::Trigger( unsigned int i )  const {

    const ptb::content::word::word_t* w = Word( i )  ;

    if ( ! w )  return nullptr ;

    return CTBFragment::Trigger( *w ) ;

  }



  // casting methods

  const ptb::content::word::feedback_t * CTBFragment::Feedback ( const ptb::content::word::word_t & w ) {

    if ( w.word_type != ptb::content::word::t_fback ) return nullptr ;

    return reinterpret_cast<const ptb::content::word::feedback_t*>( & w ) ;

  }

  const ptb::content::word::ch_status_t* CTBFragment::ChStatus ( const ptb::content::word::word_t & w ) {

    if ( w.word_type != ptb::content::word::t_ch ) return nullptr ;

    return reinterpret_cast<const ptb::content::word::ch_status_t*>( & w ) ;

  }

  const ptb::content::word::timestamp_t* CTBFragment::TSWord( const ptb::content::word::word_t & w ) {

    if ( w.word_type != ptb::content::word::t_ts ) return nullptr ;

    return reinterpret_cast<const ptb::content::word::timestamp_t*>( & w ) ;

  }

  const ptb::content::word::trigger_t*   CTBFragment::Trigger  ( const ptb::content::word::word_t & w ) {

    if ( w.word_type != ptb::content::word::t_lt && w.word_type != ptb::content::word::t_gt ) return nullptr ;

    return reinterpret_cast<const ptb::content::word::trigger_t*>( & w ) ;

  }

}  // namespace sbndaq
