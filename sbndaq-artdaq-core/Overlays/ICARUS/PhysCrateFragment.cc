#include "sbndaq-artdaq-core/Overlays/ICARUS/PhysCrateFragment.hh"
#include "cetlib_except/exception.h"
#include "sbndaq-artdaq-core/Trace/trace_defines.h"

#include <arpa/inet.h>

void icarus::PhysCrateFragmentMetadata::BoardExists(size_t i) const {
  if(i>_num_boards)
    throw cet::exception("PhysCrateFragmentMetadata::BoardExists") << 
      "Called for board " << i << " when only " << _num_boards << " boards declared.";
}

void icarus::PhysCrateFragmentMetadata::CheckNBoards(size_t i) const {
  if(i!=_num_boards)
    throw cet::exception("PhysCrateFragmentMetadata::CheckNBoards") << 
      "Failed. Asked for " << i << " when " << _num_boards << " boards declared.";
}

void icarus::PhysCrateFragment::throwIfCompressed() const {

  if(!isCompressed())
    throw cet::exception("PhysCrateFragment::DataBlock") << 
      "Asked for data block, but data may be compressed. Must decompress first.";

}

size_t icarus::PhysCrateFragment::DataTileHeaderLocation(uint16_t b) const {
  metadata()->BoardExists(b);
  size_t blocks_skipped_size=0;
  for(size_t i_b=0; i_b<b; ++i_b){
    auto dt_header = reinterpret_cast< PhysCrateDataTileHeader const *>(artdaq_Fragment_.dataBeginBytes()+blocks_skipped_size);
    blocks_skipped_size += ntohl(dt_header->packSize);
  }
  return blocks_skipped_size;
}

icarus::PhysCrateDataTileHeader const * icarus::PhysCrateFragment::DataTileHeader(uint16_t b) const {
  metadata()->BoardExists(b);
  return ( reinterpret_cast< PhysCrateDataTileHeader const *>(artdaq_Fragment_.dataBeginBytes()+DataTileHeaderLocation(b)) );
}

icarus::A2795DataBlock const * icarus::PhysCrateFragment::BoardDataBlock(uint16_t b) const {
  metadata()->BoardExists(b);
  return ( reinterpret_cast< A2795DataBlock const *>
	   (artdaq_Fragment_.dataBeginBytes() + DataTileHeaderLocation(b) + sizeof(PhysCrateDataTileHeader)));
}

icarus::A2795DataBlock::Header const& icarus::PhysCrateFragment::BoardHeader(uint16_t b) const {
  return BoardDataBlock(b)->header;
}

icarus::A2795DataBlock::header_t icarus::PhysCrateFragment::BoardEventNumber(uint16_t b) const{
  return BoardHeader(b).event_number;
}

icarus::A2795DataBlock::header_t icarus::PhysCrateFragment::BoardTimeStamp(uint16_t b) const{
  return BoardHeader(b).time_stamp;
}

icarus::A2795DataBlock::data_t const* icarus::PhysCrateFragment::BoardData(uint16_t b) const{
  //return (reinterpret_cast<A2795DataBlock::data_t const *>(BoardDataBlock(b)->data));
  if(isCompressed())
  {
    size_t cumulativeSize = (b == 0) ? 0 : cumulativeBoardSize(b - 1);
    return ( reinterpret_cast<A2795DataBlock::data_t const*>
             (artdaq_Fragment_.dataBeginBytes()
              + sizeof(PhysCrateDataTileHeader)
              + sizeof(A2795DataBlock::Header)
              + b*sizeof(PhysCrateDataTileHeader)
              + b*4*sizeof(uint16_t)
              + cumulativeSize));
  }

  return ( reinterpret_cast< A2795DataBlock::data_t const *>
	   (artdaq_Fragment_.dataBeginBytes() + sizeof(PhysCrateDataTileHeader)
	    + b*(sizeof(PhysCrateDataTileHeader) + 4*sizeof(uint16_t) + BoardBlockSize())
	    + sizeof(A2795DataBlock::Header)) );  
}

icarus::A2795DataBlock::data_t icarus::PhysCrateFragment::adc_val(size_t b,size_t c, size_t s) const{
  if (isCompressed())
    return adc_val_recursive_helper(b, c, 0, s,
             std::make_pair(static_cast<icarus::A2795DataBlock::data_t>(0), BoardData(b))).first;

  return ( *(BoardData(b)+s*nChannelsPerBoard()+c) & (~(1<<(metadata()->num_adc_bits()+1))) );
}

std::ostream & icarus::operator << (std::ostream & os, PhysCrateFragmentMetadata const& m){

  os << "\nPhysCrateFragmentMetadata:" << std::dec
     << "\n\tRun Number: " << m.run_number() 
     << "\n\tNumber of ADC bits: " << m.num_adc_bits()
     << "\n\tSamples per channel: " << m.samples_per_channel()
     << "\n\tChannels per board: " << m.channels_per_board()
     << "\n\tCompression scheme: " << m.compression_scheme()
     << "\n\tNumber of boards: " << m.num_boards();
    for(size_t i_b=0; i_b<m.num_boards(); ++i_b)
      os << "\n\t\tBoard " << i_b << " ID: " << m.board_id(i_b);
    os << std::endl;
    return os;
}

icarus::PhysCrateDataTileHeader::PhysCrateDataTileHeader( struct DataTile::Header const& h)
{

  token          = (*(reinterpret_cast<const uint32_t*>(&(h.token))));
  info1          = (*(reinterpret_cast<const uint32_t*>(&(h.info1))));
  info2          = (*(reinterpret_cast<const uint32_t*>(&(h.info2))));
  info3          = (*(reinterpret_cast<const uint32_t*>(&(h.info3))));
  timeinfo       = h.timeinfo;
  pkt_fmt_ver    = (*(reinterpret_cast<const uint32_t*>(&(h.chID))) & 0xf000)>>24;
  crate_id       = (*(reinterpret_cast<const uint32_t*>(&(h.chID))) & 0x0f00)>>16;
  board_id       = (*(reinterpret_cast<const uint32_t*>(&(h.chID))) & 0x00f0)>>8;
  board_status   = (*(reinterpret_cast<const uint32_t*>(&(h.chID))) & 0x000f);
  packSize       = (*(reinterpret_cast<const uint32_t*>(&(h.packSize))));

}

std::ostream & icarus::operator << (std::ostream & os, struct PhysCrateDataTileHeader const & h){
  os << "\nPhysCrateDataTileHeader: ";

  os << "\n\tToken: " << h.token << std::hex << "(0x" << h.token << ")" << std::dec;

  os << "\n\tInfo1: " << h.info1 << std::hex << "(0x" << h.info1 << ")" << std::dec;

  os << "\n\tInfo2: " << h.info2 << std::hex << "(0x" << h.info2 << ")" << std::dec;
  os << "\n\t\tSlotID: " << h.StatusReg_SlotID();
  os << "\n\t\tRunning: " << h.StatusReg_Running();
  os << "\n\t\tDataReady: " << h.StatusReg_DataReady();
  os << "\n\t\tBusy: " << h.StatusReg_Busy();
  os << "\n\t\tGBusy: " << h.StatusReg_GBusy();

  os << "\n\tInfo3: " << h.info3 << std::hex << "(0x" << h.info3 << ")" << std::dec;

  os << "\n\tAbsolute trigger time: " << h.timeinfo;
  os << "\n\tPacket format version: " << h.pkt_fmt_ver;
  os << "\n\tCrate ID: " << h.crate_id;
  os << "\n\tBoard ID: " << h.board_id;
  os << "\n\tBoard status: " << h.board_status;
  os << "\n\tPacket size: " << h.packSize;

  return os;
}

std::ostream & icarus::operator << (std::ostream & os, struct A2795DataBlock const & a){
  os << "\nA2795DataBlockHeader: ";
  os << "\n\tEvent Number : " << a.header.event_number;
  os << "\n\tTime stamp: " << a.header.time_stamp;
  return os;
}

std::ostream & icarus::operator << (std::ostream & os, PhysCrateFragment const & f) {
  os << "PhysCrateFragment: "
     << *(f.metadata());
  os << "\n\tNumber of boards: " << f.nBoards();
  os << "\n\tNumber of channels: " << f.nChannels();
  os << "\n\tDataPayloadSize: " << f.DataPayloadSize();
  os << "\n\tIsCompressed? : " << f.isCompressed();

  os << "\n\tBoard event number (0): " << f.BoardEventNumber();
  os << "\n\tBoard time stamp (0): " << f.BoardTimeStamp();

  os << *(f.DataTileHeader());

  if(f.isCompressed()) return os;

  os << "\n\tBoardBlockSize: " << f.BoardBlockSize();

  for(size_t i_b=0; i_b<f.nBoards(); ++i_b)
    os << "\n\t\tBoard " << i_b << f.BoardData(i_b);

  os << std::endl;
  return os;
}

bool icarus::PhysCrateFragment::Verify() const {
  bool verified=true;

  size_t expectedSize = BoardBlockSize()*nBoards();

  // If the fragment is compressed we expect 6 fewer bytes for each compressed set of 4 channel diffs
  // however if there are an odd number of compressions in a sample there are 2 additional bytes
  if (isCompressed())
    for(size_t i_b=0; i_b<nBoards(); ++i_b)
      for(size_t i_s=0; i_s<nSamplesPerChannel(); ++i_s)
        {
          size_t nCompressed = std::bitset<16>(CompressionKey(i_b, i_s)).count();
          expectedSize -= 6*nCompressed - 2*(nCompressed % 2);
        }

  if(artdaq_Fragment_.dataSizeBytes()!=expectedSize){
    TRACEN("PhysCrateFragment",TLVL_WARNING,"PhysCrateFragment::Verify : Data size not correct!\n\tObserved=%lu, Expected=%lu\n",
	  artdaq_Fragment_.dataSizeBytes(),
	  expectedSize);
    verified=false;
  }

  auto const& ev=BoardEventNumber(0);
  auto const& ts=BoardTimeStamp(0);
  bool boards_agree=true;
  for(size_t i_b=0; i_b<nBoards(); ++i_b)
    if(BoardEventNumber(i_b)!=ev || BoardTimeStamp(i_b)!=ts){
      boards_agree=false;
      break;
    }
  if(!boards_agree){
    TRACEN("PhysCrateFragment",TLVL_WARNING,"PhysCrateFragment::Verify : Boards don't agree.");
    for(size_t i_b=0; i_b<nBoards(); ++i_b)
      TRACEN("PhysCrateFragment",TLVL_WARNING,"\tBoard %lu (Event Number,Time Stamp)=(%u,%u)", 
	    i_b,BoardEventNumber(i_b),BoardTimeStamp(i_b));
    verified=false;
  }
  
  return verified;
    
}

std::vector<uint16_t> icarus::PhysCrateFragment::GenerateKeys(artdaq::Fragment const& f) {
  size_t nBoards   = f.metadata<icarus::PhysCrateFragmentMetadata>()->num_boards();
  size_t nSamples  = f.metadata<icarus::PhysCrateFragmentMetadata>()->samples_per_channel();

  std::vector<uint16_t> keys(nBoards*nSamples, 0);

  // if there's no compression it's all uncompressed
  // NOTE: double check how compression_scheme() is encoded
  if (f.metadata<icarus::PhysCrateFragmentMetadata>()->compression_scheme() == 0)
  {
    TRACEN("PhysCrateFragment",TLVL_DEBUG+3,"PhysCrateFragment::GenerateKeys : Uncompressed fragement. Return trivial keys.");
    return keys;
  }

  // loop over the fragment data and check the compression
  // each board/sample pair gets one 16-bit key
  // each bit corresponds to a block of 4 channels
  // 0 means those channels are uncompressed, 1 means they are compressed
  // if the 4 most significant bits of the 16-bit word is 0x8 the words is compressed
  size_t cumulativePrevBlockSize = 0;
  for (size_t b = 0; b < nBoards; b++)
  {
    icarus::A2795DataBlock::data_t const* boardData
                                            = reinterpret_cast<icarus::A2795DataBlock::data_t const*>
                                                       ( f.dataBeginBytes()
                                                       + sizeof(icarus::PhysCrateDataTileHeader)
                                                       + sizeof(icarus::A2795DataBlock::Header)
                                                       + b*sizeof(icarus::PhysCrateDataTileHeader)
                                                       + b*4*sizeof(uint16_t)
                                                       + cumulativePrevBlockSize                   );
    size_t nWord = 0;
    for (size_t s = 0; s < nSamples; s++)
    {
      uint16_t key = 0;
      for (size_t bit = 0; bit < 16; bit++)
      {
        icarus::A2795DataBlock::data_t word = boardData[nWord];
        bool isCompressed = ((word & 0xF000) != 0x8000);
        key += (isCompressed << bit);
        nWord += (isCompressed) ? 1 : 4;
      }
      keys[b*nSamples + s] = key;
      nWord += (std::bitset<16>(key).count() % 2);
      cumulativePrevBlockSize += icarus::PhysCrateFragment::SampleBytesFromKey(key);
      TRACEN("PhysCrateFragment",TLVL_DEBUG+3,"PhysCrateFragment::GenerateKeys : Compression key for board %d, sample %d is %s", b, s, std::bitset<16>(key).to_string().c_str());
    }
  }
  return keys;
}

icarus::PhysCrateFragment::recursionPair icarus::PhysCrateFragment::adc_val_recursive_helper
  (size_t b, size_t c, size_t s, size_t sTarget, icarus::PhysCrateFragment::recursionPair pair) const
{
  // this function helps calculate the ADC values from compressed fragments
  // it is tail recursive and accumulates the ADC values while finding each ADC difference
  icarus::A2795DataBlock::data_t runningVal = pair.first;
  const icarus::A2795DataBlock::data_t* loc = pair.second;
  const size_t cSet = c / 4;
  const size_t channelInSet = c % 4;
  uint16_t sampleKey = this->CompressionKey(b, s);

  size_t cIndex = 4*cSet - 3*std::bitset<16>(sampleKey % (1 << cSet)).count();
  bool isChannelSetCompressed = ((sampleKey >> cSet) & 0x0001) == 0x0001;

  if (isChannelSetCompressed)
  {
    icarus::A2795DataBlock::data_t dataSlice = *(loc+cIndex);
    icarus::A2795DataBlock::data_t fourBitDiff = (dataSlice >> (4*channelInSet)) & 0x000F;
    bool isNeg = (fourBitDiff >> 3);
    runningVal += isNeg*0xFFF0 + fourBitDiff;
  } else {
    icarus::A2795DataBlock::data_t dataSlice = *(loc+cIndex+channelInSet);
    icarus::A2795DataBlock::data_t twelveBitDiff = dataSlice & 0x0FFF;
    bool isNeg = (s != 0) && (twelveBitDiff >> 11);
    runningVal += isNeg*0xF000 + twelveBitDiff;
  }

  size_t keyCount = std::bitset<16>(sampleKey).count();
  size_t increment = 64 - 3*keyCount + (keyCount % 2);

  if (s == sTarget)
    return std::make_pair(runningVal, loc + increment);

  return this->adc_val_recursive_helper(b, c, s + 1, sTarget, std::make_pair(runningVal, loc + increment));
}
