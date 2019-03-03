#include "sbndaq-artdaq-core/Overlays/SBND/CTBFragment.hh"

namespace sbndaq {


  std::ostream & operator << (std::ostream & out, CTBFragment const & f ) {

    out << "Fragment size: " << f.artdaq_Fragment_.dataSizeBytes() << " -> " << f._n_words << " words" << std::endl ;

    for ( unsigned int i = 0; i < f.NWords() ; ++i ) {

      if ( f.Trigger(i) ) {
        out << "Trigger word " << std::hex << f.Trigger(i) -> word_type
            << ", payload: "  << f.Trigger(i) -> trigger_word
            << ", TS: " << f.Trigger(i) -> timestamp << std::dec << std::endl ;
      }
      else if ( f.ChStatus(i) ) {
	out << "Check Status word " << std::hex 
            << " PDS " << f.ChStatus(i) -> get_pds() 
            << ", CRT: " << f.ChStatus(i) -> get_crt()
	    << ", Beam: " << f.ChStatus(i) -> get_beam() 
	    << ", TS: " << f.ChStatus(i) -> timestamp
	    << std::dec << std::endl ;

      }
      else if ( f.Feedback(i) ) {
	out << "Feedback word  " << std::hex 
	    << ", Padding: " << f.Feedback(i) -> padding
	    << ", Source: " << f.Feedback(i) -> source
	    << ", Code: " << f.Feedback(i) -> code
            << ", TS: " << f.Feedback(i) -> timestamp << std::dec << std::endl ;

      }
      else {
	out << "type: " << std::hex << f.Word(i) -> word_type 
	    << ", payload: " << f.Word(i) -> payload 
	    << ", TS: " << f.Word(i) -> timestamp << std::dec << std::endl ;
      }

    }


    return out ;
  }



  CTBFragment::CTBFragment( artdaq::Fragment const & f ) : 

    _n_words( f.dataSizeBytes()/CTBFragment::WordSize() ),
    artdaq_Fragment_( f ) 
  { ; } 


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

 
  const ptb::content::word::timestamp_t* CTBFragment::Timestamp( unsigned int i ) const {

    const ptb::content::word::word_t* w = Word( i )  ;

    if ( ! w )  return nullptr ;

    return CTBFragment::Timestamp( *w ) ;

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

  const ptb::content::word::timestamp_t* CTBFragment::Timestamp( const ptb::content::word::word_t & w ) {
  
    if ( w.word_type != ptb::content::word::t_ts ) return nullptr ;

    return reinterpret_cast<const ptb::content::word::timestamp_t*>( & w ) ;

  }

  const ptb::content::word::trigger_t*   CTBFragment::Trigger  ( const ptb::content::word::word_t & w ) {

    if ( w.word_type != ptb::content::word::t_lt && w.word_type != ptb::content::word::t_gt ) return nullptr ;

    return reinterpret_cast<const ptb::content::word::trigger_t*>( & w ) ;

  }

}  // namespace sbndaq

