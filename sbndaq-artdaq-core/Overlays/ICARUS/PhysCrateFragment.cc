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
              + (1+b)*sizeof(PhysCrateDataTileHeader)
              + (1+b)*sizeof(A2795DataBlock::Header)
              + 4*b*sizeof(uint16_t)
              + cumulativeSize));
  }

  return ( reinterpret_cast< A2795DataBlock::data_t const *>
	   (artdaq_Fragment_.dataBeginBytes() + sizeof(PhysCrateDataTileHeader)
	    + b*(sizeof(PhysCrateDataTileHeader) + 4*sizeof(uint16_t) + BoardBlockSize())
	    + sizeof(A2795DataBlock::Header)) );  
}

icarus::A2795DataBlock::data_t icarus::PhysCrateFragment::adc_val(size_t b,size_t c, size_t s) const{
  //if (isCompressed())
  //  return adc_val_recursive_helper(b, c, 0, s,
  //           std::make_pair(static_cast<icarus::A2795DataBlock::data_t>(0), BoardData(b))).first;

  return (isCompressed()) ? this->adcVal_fromAccessor(b, c, s) :
                            ( *(BoardData(b)+s*nChannelsPerBoard()+c) & (~(1<<(metadata()->num_adc_bits()+1))) );
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
          icarus::PhysCrateFragment::Key const& compKey = CompressionKey(i_b, i_s);
          size_t nCompressed = 0;
          for (size_t cB = 0; cB < 16; ++cB)
            nCompressed += compKey[cB];

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

std::pair<std::vector<icarus::PhysCrateFragment::Key>, std::vector<icarus::A2795DataBlock::data_t>> icarus::PhysCrateFragment::GenerateAccessors(artdaq::Fragment const& f) {
  size_t nBoards   = f.metadata<icarus::PhysCrateFragmentMetadata>()->num_boards();
  size_t nChannels = f.metadata<icarus::PhysCrateFragmentMetadata>()->channels_per_board();
  size_t nSamples  = f.metadata<icarus::PhysCrateFragmentMetadata>()->samples_per_channel();

  std::vector<icarus::PhysCrateFragment::Key> keys;
  std::vector<icarus::A2795DataBlock::data_t> adcValues(nBoards*nSamples*nChannels, 0);

  // if there's no compression it's all uncompressed
  if (f.metadata<icarus::PhysCrateFragmentMetadata>()->compression_scheme() == 0)
  {
    for (size_t i_key = 0; i_key < nBoards*nSamples; ++i_key)
      keys.push_back({});

    size_t cumulativePrevBlockSize = 0;
    for (size_t b = 0; b < nBoards; ++b)
    {
      icarus::A2795DataBlock::data_t const* boardData
                                            = reinterpret_cast<icarus::A2795DataBlock::data_t const*>
                                                       ( f.dataBeginBytes()
                                                       + (1+b)*sizeof(icarus::PhysCrateDataTileHeader)
                                                       + (1+b)*sizeof(icarus::A2795DataBlock::Header)
                                                       + 4*b*sizeof(uint16_t)
                                                       + cumulativePrevBlockSize                     );
      for (size_t c = 0; c < nChannels; ++c)
      {
        for (size_t s = 0; s < nSamples; ++s)
          adcValues[b*nSamples*nChannels + c*nSamples + s] = (*(boardData+s*nChannels+c) & (~(1<<(f.metadata<icarus::PhysCrateFragmentMetadata>()->num_adc_bits()+1))));
      }
      cumulativePrevBlockSize += icarus::PhysCrateFragment::SampleBytesFromKey({});
    }

    return std::make_pair(keys, adcValues);
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
                                                       + (1+b)*sizeof(icarus::PhysCrateDataTileHeader)
                                                       + (1+b)*sizeof(icarus::A2795DataBlock::Header)
                                                       + 4*b*sizeof(uint16_t)
                                                       + cumulativePrevBlockSize                     );
    size_t nWord = 0;
    for (size_t s = 0; s < nSamples; s++)
    {
      icarus::PhysCrateFragment::Key key = {};
      size_t keyCount = 0;
      for (size_t bit = 0; bit < nChannels/4; bit++)
      {
        icarus::A2795DataBlock::data_t word = boardData[nWord];
        bool isCompressed = ((word & 0xF000) != 0x8000);
        for (size_t cInSet = 0; cInSet < 4; ++ cInSet)
        {
          size_t c = 4*bit + cInSet;
          if (isCompressed)
          {
            icarus::A2795DataBlock::data_t fourBitDiff = (word >> (4*cInSet)) & 0x000F;
            bool isNeg = (fourBitDiff >> 3);
            icarus::A2795DataBlock::data_t prevSample = (s != 0) ? adcValues[b*nSamples*nChannels + c*nSamples + s - 1] : 0;
            adcValues[b*nSamples*nChannels + c*nSamples + s] = (isNeg*0xFFF0 + fourBitDiff + prevSample);
          } else {
            icarus::A2795DataBlock::data_t twelveBitDiff = (boardData[nWord + cInSet] & 0x0FFF);
            bool isNeg = (s != 0) && (twelveBitDiff >> 11);
            icarus::A2795DataBlock::data_t prevSample = (s != 0) ? adcValues[b*nSamples*nChannels + c*nSamples + s - 1] : 0;
            adcValues[b*nSamples*nChannels + c*nSamples + s] = (isNeg*0xF000 + twelveBitDiff + prevSample);
          }
        }
        nWord += (isCompressed) ? 1 : 4;
        keyCount += isCompressed;
        key[bit] = isCompressed;
      }
      keys.push_back(key);
      nWord += (keyCount % 2);
      cumulativePrevBlockSize += icarus::PhysCrateFragment::SampleBytesFromKey(key);
    }
  }
  return std::make_pair(keys, adcValues);
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
  icarus::PhysCrateFragment::Key const& sampleKey = this->CompressionKey(b, s);
  size_t keyiPartialCount = 0;
  for (size_t cB = 0; cB < cSet; ++cB)
    keyiPartialCount += sampleKey[cB];

  size_t cIndex = 4*cSet - 3*keyiPartialCount;
  bool isChannelSetCompressed = sampleKey[cSet];

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

  size_t keyCount = keyiPartialCount;
  for (size_t cB = cSet; cB < 16; ++cB)
    keyCount += sampleKey[cB];

  size_t increment = 64 - 3*keyCount + (keyCount % 2);

  if (s == sTarget)
    return std::make_pair(runningVal, loc + increment);

  return this->adc_val_recursive_helper(b, c, s + 1, sTarget, std::make_pair(runningVal, loc + increment));
}

artdaq::Fragment icarus::PhysCrateFragment::compressArtdaqFragment(artdaq::Fragment const & f)
{
  // you shouldn't run this if you already have a compressed fragment
  // but in case you do it shouldn't bother with the calculations
  if (f.metadata<icarus::PhysCrateFragmentMetadata>()->compression_scheme() != 0)
    return f;

  // use an overlay to help access old fragment
  icarus::PhysCrateFragment overlay(f);

  // calculate how many bytes we should save
  size_t savedBytes = 0;
  for (size_t board = 0; board < overlay.nBoards(); ++board)
  {
    // save if we compress an odd number of times
    bool oddCompressions = false;
    // we don't save any bytes on the 0th sample
    for (size_t sample = 1; sample < overlay.nSamplesPerChannel(); ++sample)
    {
      for (size_t channelBlock = 0; channelBlock < overlay.nChannelsPerBoard() / 4; ++channelBlock)
      {
        if (std::abs(overlay.adc_val(board, 4*channelBlock + 0, sample) - overlay.adc_val(board, 4*channelBlock + 0, sample - 1)) < 8 &&
            std::abs(overlay.adc_val(board, 4*channelBlock + 1, sample) - overlay.adc_val(board, 4*channelBlock + 1, sample - 1)) < 8 &&
            std::abs(overlay.adc_val(board, 4*channelBlock + 2, sample) - overlay.adc_val(board, 4*channelBlock + 2, sample - 1)) < 8 &&
            std::abs(overlay.adc_val(board, 4*channelBlock + 3, sample) - overlay.adc_val(board, 4*channelBlock + 3, sample - 1)) < 8 )
        {
          // each compression saves 6 bytes (4*16 bits - 16 bits = 48 bits = 6 bytes)
          savedBytes += 6;
          oddCompressions = (not oddCompressions);
        }
      }
    }
    // acount for spacer if needed
    if (oddCompressions)
      savedBytes -= 2;
  }

  // create a new fragment to return
  // resize it based on home many compressions occur
  artdaq::Fragment compressed_fragment(f);
  compressed_fragment.resizeBytes(f.dataSizeBytes() - savedBytes);

  // get a pointer to the start of the new payload
  // and to the start of the old payload
  icarus::A2795DataBlock::data_t*         compressedDataStart = reinterpret_cast<A2795DataBlock::data_t*>      (compressed_fragment.dataBeginBytes());
  icarus::A2795DataBlock::data_t const* uncompressedDataStart = reinterpret_cast<A2795DataBlock::data_t const*>(                  f.dataBeginBytes());

  // go through board/channel/sample and load up the data
  // data is stored in channel->sample->board
  // keep track of where we are reading from/writing to
  size_t   compressedDataOffset = 0;
  size_t uncompressedDataOffset = 0;
  for (size_t board = 0; board < overlay.nBoards(); ++board)
  {
    // each board has a header...
    for (size_t boardHeaderWord = 0; boardHeaderWord < (sizeof(PhysCrateDataTileHeader) + sizeof(A2795DataBlock::Header)) / sizeof(uint16_t); ++boardHeaderWord)
    {
      *(compressedDataStart + compressedDataOffset) = *(uncompressedDataStart + uncompressedDataOffset);
      ++compressedDataOffset;
      ++uncompressedDataOffset;
    }
    // since the 0th sample is used as a reference, store it as is
    for (size_t channel = 0; channel < overlay.nChannelsPerBoard(); ++channel)
    {
      *(compressedDataStart + compressedDataOffset) = (overlay.adc_val(board, channel, 0) & 0x0FFF) + 0x8000;
      ++compressedDataOffset;
      ++uncompressedDataOffset;
    }
    // from here on we store the difference between samples for each channel
    for (size_t sample = 1; sample < overlay.nSamplesPerChannel(); ++sample)
    {
      bool oddCompressions = false;
      // work in blocks of 4 channels
      for (size_t channelBlock = 0; channelBlock < overlay.nChannelsPerBoard() / 4; ++channelBlock)
      {
        int16_t adcDiff_0 = overlay.adc_val(board, 4*channelBlock + 0, sample) - overlay.adc_val(board, 4*channelBlock + 0, sample - 1);
        int16_t adcDiff_1 = overlay.adc_val(board, 4*channelBlock + 1, sample) - overlay.adc_val(board, 4*channelBlock + 1, sample - 1);
        int16_t adcDiff_2 = overlay.adc_val(board, 4*channelBlock + 2, sample) - overlay.adc_val(board, 4*channelBlock + 2, sample - 1);
        int16_t adcDiff_3 = overlay.adc_val(board, 4*channelBlock + 3, sample) - overlay.adc_val(board, 4*channelBlock + 3, sample - 1);
        uncompressedDataOffset += 4;
        // if the 4 diffs are all less than 8 in magnetude then we compress the block
        if (std::abs(adcDiff_0) < 8 && 
            std::abs(adcDiff_1) < 8 &&
            std::abs(adcDiff_2) < 8 &&
            std::abs(adcDiff_3) < 8 )
        {
          oddCompressions = (not oddCompressions);
          *(compressedDataStart + compressedDataOffset) = ((adcDiff_0 & 0x000F) <<  0) +
                                                          ((adcDiff_1 & 0x000F) <<  4) +
                                                          ((adcDiff_2 & 0x000F) <<  8) +
                                                          ((adcDiff_3 & 0x000F) << 12) ;
          ++compressedDataOffset;
        } else {
          *(compressedDataStart + compressedDataOffset + 0) = (adcDiff_0 & 0x0FFF) + 0x8000;
          *(compressedDataStart + compressedDataOffset + 1) = (adcDiff_1 & 0x0FFF) + 0x8000;
          *(compressedDataStart + compressedDataOffset + 2) = (adcDiff_2 & 0x0FFF) + 0x8000;
          *(compressedDataStart + compressedDataOffset + 3) = (adcDiff_3 & 0x0FFF) + 0x8000;
          compressedDataOffset += 4;
        }
      }
      // if there are an odd number of words in the difference then there needs to be a spacer added
      if (oddCompressions)
      {
        *(compressedDataStart + compressedDataOffset) = 0;
        ++compressedDataOffset;
      }
    }
    // ...and each board has a trailer
    for (size_t boardTrailerWord = 0; boardTrailerWord < 4; ++boardTrailerWord)
    {
      *(compressedDataStart + compressedDataOffset) = *(uncompressedDataStart + uncompressedDataOffset);
      ++compressedDataOffset;
      ++uncompressedDataOffset;
    }
  }
  
  // updated the metadata to reflect the compression
  if (compressed_fragment.hasMetadata()){
    icarus::PhysCrateFragmentMetadata::data_t runNumber = f.metadata<icarus::PhysCrateFragmentMetadata>()->run_number();
    icarus::PhysCrateFragmentMetadata::data_t nBoards   = f.metadata<icarus::PhysCrateFragmentMetadata>()->num_boards();
    icarus::PhysCrateFragmentMetadata::data_t cPerB     = f.metadata<icarus::PhysCrateFragmentMetadata>()->channels_per_board();
    icarus::PhysCrateFragmentMetadata::data_t sPerC     = f.metadata<icarus::PhysCrateFragmentMetadata>()->samples_per_channel();
    icarus::PhysCrateFragmentMetadata::data_t adcPerS   = f.metadata<icarus::PhysCrateFragmentMetadata>()->num_adc_bits(); // i don't know why the names are like this...
    icarus::PhysCrateFragmentMetadata::data_t compress  = 1;
    std::vector<icarus::PhysCrateFragmentMetadata::id_t> boardIds(nBoards);
    icarus::PhysCrateFragmentMetadata updatedMD(runNumber, nBoards, cPerB, sPerC, adcPerS, compress, boardIds);
    compressed_fragment.updateMetadata<icarus::PhysCrateFragmentMetadata>(updatedMD);
  }

  return compressed_fragment;
}

artdaq::Fragment icarus::PhysCrateFragment::decompressArtdaqFragment(artdaq::Fragment const & f)
{
  // you shouldn't run this if you already have a decompressed fragment
  // but in case you do it shouldn't bother with the calculations
  if (f.metadata<icarus::PhysCrateFragmentMetadata>()->compression_scheme() == 0)
    return f;

  // use an overlay to help access old fragment
  icarus::PhysCrateFragment overlay(f);

  // calculate how many bytes we should add
  size_t savedBytes = 0;
  for (size_t board = 0; board < overlay.nBoards(); ++board)
  {
    // save if we compress an odd number of times
    bool oddCompressions = false;
    // we don't save any bytes on the 0th sample
    for (size_t sample = 1; sample < overlay.nSamplesPerChannel(); ++sample)
    {
      for (auto const& bit : overlay.CompressionKey(board, sample))
      {
        if (bit)
        {
          // each compression saves 6 bytes (4*16 bits - 16 bits = 48 bits = 6 bytes)
          savedBytes += 6;
          oddCompressions = (not oddCompressions);
        }
      }
    }
    // acount for spacer if needed
    if (oddCompressions)
      savedBytes -= 2;
  }

  // create a new fragment to return
  // resize it based on home many compressions occur
  artdaq::Fragment decompressed_fragment(f);
  decompressed_fragment.resizeBytes(f.dataSizeBytes() + savedBytes);

  // get a pointer to the start of the new payload
  // and to the start of the old payload
  icarus::A2795DataBlock::data_t*       decompressedDataStart = reinterpret_cast<A2795DataBlock::data_t*>      (decompressed_fragment.dataBeginBytes());
  icarus::A2795DataBlock::data_t const*   compressedDataStart = reinterpret_cast<A2795DataBlock::data_t const*>(                    f.dataBeginBytes());

  // go through board/channel/sample and load up the data
  // data is stored in channel->sample->board
  // keep track of where we are reading from/writing to
  size_t decompressedDataOffset = 0;
  size_t   compressedDataOffset = 0;
  for (size_t board = 0; board < overlay.nBoards(); ++board)
  {
    // each board has a header...
    for (size_t boardHeaderWord = 0; boardHeaderWord < (sizeof(PhysCrateDataTileHeader) + sizeof(A2795DataBlock::Header)) / sizeof(uint16_t); ++boardHeaderWord)
    {
      *(decompressedDataStart + decompressedDataOffset) = *(compressedDataStart + compressedDataOffset);
      ++decompressedDataOffset;
      ++compressedDataOffset;
    }
    for (size_t sample = 0; sample < overlay.nSamplesPerChannel(); ++sample)
    {
      for (size_t channel = 0; channel < overlay.nChannelsPerBoard(); ++channel)
      {
        *(decompressedDataStart + decompressedDataOffset) = overlay.adc_val(board, channel, sample);
        ++decompressedDataOffset;
      }
      bool oddCompressions = false;
      for (auto const& bit : overlay.CompressionKey(board, sample))
      {
        if (bit)
        {
          oddCompressions = (not oddCompressions);
          ++compressedDataOffset;
        } else {
          compressedDataOffset += 4;
        }
      }
      if (oddCompressions)
        ++compressedDataOffset;
    }
    // ...and each board has a trailer
    for (size_t boardTrailerWord = 0; boardTrailerWord < 4; ++boardTrailerWord)
    {
      *(decompressedDataStart + decompressedDataOffset) = *(compressedDataStart + compressedDataOffset);
      ++decompressedDataOffset;
      ++compressedDataOffset;
    }
  }
  
  // updated the metadata to reflect the compression
  if (decompressed_fragment.hasMetadata()){
    icarus::PhysCrateFragmentMetadata::data_t runNumber = f.metadata<icarus::PhysCrateFragmentMetadata>()->run_number();
    icarus::PhysCrateFragmentMetadata::data_t nBoards   = f.metadata<icarus::PhysCrateFragmentMetadata>()->num_boards();
    icarus::PhysCrateFragmentMetadata::data_t cPerB     = f.metadata<icarus::PhysCrateFragmentMetadata>()->channels_per_board();
    icarus::PhysCrateFragmentMetadata::data_t sPerC     = f.metadata<icarus::PhysCrateFragmentMetadata>()->samples_per_channel();
    icarus::PhysCrateFragmentMetadata::data_t adcPerS   = f.metadata<icarus::PhysCrateFragmentMetadata>()->num_adc_bits(); // i don't know why the names are like this...
    icarus::PhysCrateFragmentMetadata::data_t compress  = 0;
    std::vector<icarus::PhysCrateFragmentMetadata::id_t> boardIds(nBoards);
    icarus::PhysCrateFragmentMetadata updatedMD(runNumber, nBoards, cPerB, sPerC, adcPerS, compress, boardIds);
    decompressed_fragment.updateMetadata<icarus::PhysCrateFragmentMetadata>(updatedMD);
  }

  return decompressed_fragment;
}
