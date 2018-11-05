#ifndef sbndaq_artdaq_core_Overlays_Common_Utilities_hh
#define sbndaq_artdaq_core_Overlays_Common_Utilities_hh

#include "cetlib/exception.h"

#include <string>

namespace sbndaq {

// JCF, Jul-21-2015

// display_bits() will display the entire bit-by-bit contents of
// nybtes of memory beginning at memstart; the optional last argument,
// "sourcename", is the label you want associated with the diagnostic
// message (e.g., "PennReceiver")

void display_bits(void* memstart, size_t nbytes, std::string sourcename = "sbndaq_display_bits");


// JCF, Jul-16-2015                                                                                                  

// reinterpret_cast_checked will double check that the pointer we                                                    
// cast to actually points to the same location in memory as the                                                     
// pointer we cast from. This almost certainly is the case, but it's                                                 
// not an ironclad guarantee in the C++ standard, unfortunately.    

template <typename S, typename T>
S reinterpret_cast_checked(T inptr) {

  S outptr = reinterpret_cast<S>( inptr );

  const void* inptr_void = static_cast<const void*>( inptr );
  const void* outptr_void = static_cast<const void*>( outptr );

  if ( inptr_void != outptr_void ) {
    throw cet::exception("reinterpret_cast_checked") << "Error: reinterpret_cast casted " << inptr_void << " to " << outptr_void << " => results of the cast can't be trusted";
  }

  return outptr;
}

}

#endif
