#define TRACE_NAME "NevisTPCUtilities"
#include "trace.h"

#include "sbndaq-artdaq-core/Overlays/SBND/NevisTPC/NevisTPCUtilities.hh"
#include <iostream>

sbndaq::NevisTPCWordType_t sbndaq::NevisTPCDecoder::get_word_type( uint16_t word )
{
  if( (word & 0xF000) == 0x4000 ) return NevisTPCWordType_t::kChannelHeader;
  if( (word & 0xF000) == 0x0000 ) return NevisTPCWordType_t::kADC;
  if( (word & 0xC000) == 0x8000 ) return NevisTPCWordType_t::kADCHuffman; // Look for headers first
  if( (word & 0xF000) == 0x5000 ) return NevisTPCWordType_t::kChannelEnding;
  else 
    return NevisTPCWordType_t::kUnknown;
}

int sbndaq::NevisTPCDecoder::decode_huffman( int zeros )
{
  switch( zeros ){
  case 0: return 0;
  case 1: return -1;
  case 2: return 1;
  case 3: return -2;
  case 4: return 2;
  case 5: return -3;
  case 6: return 3;
  default:
    TRACE(TERROR,"ERROR: Number of zeros %d in Huffman word is out of bounds",zeros);
    return 4096;
  }
}

sbndaq::NevisTPCHeader sbndaq::NevisTPCDecoder::decode_header(const char* hdr_ptr)
{
  return *(reinterpret_cast<NevisTPCHeader const*>(hdr_ptr));
}

size_t sbndaq::NevisTPCDecoder::decode_data(const sbndaq::NevisTPC_ADC_t* data_ptr,
					     size_t n_words,
					     std::unordered_map<uint16_t,sbndaq::NevisTPC_Data_t> & wvfm_map)
{
  //clear out the output map
  wvfm_map.clear();

  //some initialization so we don't do this in the loops
  const NevisTPC_ADC_t* w_ptr;
  uint16_t currentChannel = 0;
  NevisTPC_Data_t* currentWaveformPtr;
  std::pair< std::unordered_map<uint16_t,NevisTPC_Data_t>::iterator, bool> emplace_pair;
  uint16_t mask;
  size_t i_diff;

  //huffman decoding stuff
  int zeros = 0; // Counter of 0s interleaved between 1s
  std::vector<int> differences; // Vector with Huffman-decoded differences
  differences.reserve(14);

  for(size_t i_w=0; i_w<n_words; ++i_w){
    w_ptr = data_ptr+i_w;

    switch(get_word_type(*w_ptr)){

    case NevisTPCWordType_t::kChannelHeader:

      currentChannel = (*w_ptr & 0x3F);
      TRACE(TDECODE,"Reading channel %u ...",currentChannel);

      emplace_pair = wvfm_map.emplace(currentChannel,NevisTPC_Data_t());
      currentWaveformPtr = &((emplace_pair.first)->second);

      if(!(emplace_pair.second)){
	TRACE(TWARNING,"Previously found channel %u. Overwriting old data!",currentChannel);
	currentWaveformPtr->clear();
      }
      currentWaveformPtr->reserve(fReserveWvfmSize);
      
      break;

    case NevisTPCWordType_t::kADC:
      currentWaveformPtr->emplace_back( (*w_ptr & 0xFFF) );
      break;

    case NevisTPCWordType_t::kADCHuffman: { // Brackets needed to declare variables
      zeros=0;
      differences.clear();
      differences.reserve(14); // Speed up: reserve for the maximum number of differences stored in a Huffman word

      // Read the lowest 14 bits from left to right
      for(mask = 0x2000; mask > 0x0; mask = (mask>>1)){
	if( (*w_ptr & mask) == mask ){ // Found 1
	  differences.emplace_back( decode_huffman(zeros) );
	  zeros = 0; // Reset counter
	} else ++zeros;
      }
      // Differences are time-ordered from right to left
      for(i_diff = differences.size(); i_diff > 0; i_diff--){
	currentWaveformPtr->emplace_back( currentWaveformPtr->back() + differences[i_diff - 1] );
      }

    }break;
    case NevisTPCWordType_t::kChannelEnding:

      if( (*w_ptr & 0x3F) == currentChannel )
	TRACE(TDECODE,"Finished reading channel %u. %lu ADC samples read.",currentChannel,currentWaveformPtr->size());
      else
	TRACE(TERROR,"ERROR: Channel header %u and ending %u do not match.",currentChannel,(*w_ptr & 0x3F));

      break;

    case NevisTPCWordType_t::kUnknown:
      TRACE(TDECODE,"Found unknown word %u.",*w_ptr);

      break;
    }//endswitch


  }//end loop over words

  return wvfm_map.size();
}
