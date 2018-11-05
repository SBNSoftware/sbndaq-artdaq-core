#include "sbndaq-artdaq-core/Overlays/Common/Utilities.hh"

#include "messagefacility/MessageLogger/MessageLogger.h"

#include <sstream>
#include <bitset>

namespace sbndaq {

void display_bits(void* memstart, size_t nbytes, std::string sourcename) {

  std::stringstream bitstr;
  bitstr << "The " << nbytes << "-byte chunk of memory beginning at " << static_cast<void*>(memstart) << " is : ";

  // -- NFB : Nov-20-2015
  // reversed the order of the bits when printing, so that the msb remain on the left
  for(unsigned int i = 0; i < nbytes; i++) {
    // bitstr << std::bitset<8>(*((sbnddaq::reinterpret_cast_checked<uint8_t*>(memstart))+(nbytes-i-1))) << " ";
    bitstr << std::bitset<8>(*((sbndaq::reinterpret_cast_checked<uint8_t*>(memstart))+i)) << " ";
  }

  mf::LogInfo(sourcename.c_str()) << bitstr.str();
}

}
