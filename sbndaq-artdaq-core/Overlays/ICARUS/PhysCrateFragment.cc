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
  for (size_t i_b=0; i_b<b; ++i_b){
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
  if (b > this->nBoards() || c > this->nChannelsPerBoard() || s > this->nSamplesPerChannel())
  {
    std::cout << "WARNING in icarus::PhysCrateFragment::adc_val" << '\n'
              << "  Attempting to find ADC value for board " << b << ", channel " << c << ", sample " << s << '\n'
              << "  But Fragment only has " << this->nBoards() << " boards, " << this->nChannelsPerBoard() << " channels per board, and " << this->nSamplesPerChannel() << " samples per channel" << std::endl;
  }
  return (this->isCompressed()) ? this->adcVal_fromAccessor(b, c, s) :
                                  ( *(this->BoardData(b)+s*this->nChannelsPerBoard()+c) & (~(1<<(this->metadata()->num_adc_bits()+1))) );
}

icarus::A2795DataBlock::data_t icarus::PhysCrateFragment::adc_val_usingIterator(size_t b,size_t c, size_t s) const
{
  size_t bytesUntilBlock = (1+b)*sizeof(PhysCrateDataTileHeader)
                           + b*(BoardBlockSize() + 4*sizeof(uint16_t))
                           + sizeof(A2795DataBlock::Header);
  size_t wordsUntilBlock = (bytesUntilBlock / sizeof(icarus::A2795DataBlock::data_t));
  size_t wordsIntoBlock  = s*this->nChannelsPerBoard()+c;
  icarus::A2795DataBlock::data_t word = getA2795Word(artdaq_Fragment_, wordsUntilBlock + wordsIntoBlock);
  return (word & (~(1<<(this->metadata()->num_adc_bits()+1))) );
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
  std::vector<icarus::A2795DataBlock::data_t> adcValues(nBoards*nChannels*nSamples, std::numeric_limits<icarus::A2795DataBlock::data_t>::min());

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
          adcValues.push_back(*(boardData+s*nChannels+c) & (~(1<<(f.metadata<icarus::PhysCrateFragmentMetadata>()->num_adc_bits()+1))));
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
  uint64_t fragWord = 0;
  for (size_t b = 0; b < nBoards; b++)
  {
    // skip board headers...
    size_t headerBytes = sizeof(icarus::PhysCrateDataTileHeader) + sizeof(A2795DataBlock::Header);
    fragWord += (headerBytes / sizeof(icarus::A2795DataBlock::data_t));
    for (size_t s = 0; s < nSamples; s++)
    {
      icarus::PhysCrateFragment::Key key = {};
      size_t keyCount = 0;
      for (size_t bit = 0; bit < nChannels/4; bit++)
      {
        icarus::A2795DataBlock::data_t word = getA2795Word(f, fragWord);
        bool isCompressed = (((word & 0xF000) != 0x8000));
        for (size_t cInSet = 0; cInSet < 4; ++ cInSet)
        {
          size_t c = 4*bit + cInSet;
          if (not isCompressed)
          {
            icarus::A2795DataBlock::data_t tempWord = getA2795Word(f, fragWord + cInSet);
            int16_t twelveBitDiff = (tempWord & 0x0FFF);
            bool isNeg = (twelveBitDiff >> 11) && (s != 0);
            icarus::A2795DataBlock::data_t prevSample = (s != 0) ? adcValues[b*nSamples*nChannels + c*nSamples + s - 1] : 0;
            icarus::A2795DataBlock::data_t currSample = (isNeg*0xF000 + twelveBitDiff + prevSample);
            adcValues.at(b*nSamples*nChannels + c*nSamples + s) = currSample;
          } else {
            int16_t fourBitDiff = (word >> (4*cInSet)) & 0x000F;
            bool isNeg = (fourBitDiff >> 3);
            icarus::A2795DataBlock::data_t prevSample = (s != 0) ? adcValues[b*nSamples*nChannels + c*nSamples + s - 1] : 0;
            icarus::A2795DataBlock::data_t currSample = (isNeg*0xFFF0 + fourBitDiff + prevSample);
            adcValues.at(b*nSamples*nChannels + c*nSamples + s) = currSample;
          }
        }
        fragWord += (isCompressed) ? 1 : 4;
        keyCount += isCompressed;
        key[bit] = isCompressed;
      }
      keys.push_back(key);
      if ((keyCount % 2) == 1)
      {
        fragWord += 1;
      }
    }
    // ...and skip board trailers
   fragWord += 4;
  }
  return std::make_pair(keys, adcValues);
}

artdaq::Fragment icarus::PhysCrateFragment::compressArtdaqFragment(artdaq::Fragment const & f)
{
  // you shouldn't run this if you already have a compressed fragment
  // but in case you do it shouldn't bother with the calculations
  if (f.metadata<icarus::PhysCrateFragmentMetadata>()->compression_scheme() != 0)
    return f;

  // use an overlay to help access old fragment
  icarus::PhysCrateFragment overlay(f);

  // create a new fragment to return
  artdaq::Fragment compressed_fragment(f);

  // go through board/channel/sample and load up the data
  // data is stored in channel->sample->board
  // keep track of where we are reading from/writing to
  size_t   compressedDataOffset = 0;
  size_t uncompressedDataOffset = 0;
  std::vector<icarus::PhysCrateFragment::Key> compressionKeys;
  compressionKeys.resize(overlay.nBoards() * overlay.nSamplesPerChannel());
  size_t totalDataTileSize = 0;
  for (size_t board = 0; board < overlay.nBoards(); ++board)
  {
    // each board has a header...
    uint32_t boardDataTileSize = sizeof(PhysCrateDataTileHeader) + sizeof(A2795DataBlock::Header);
    for (size_t boardHeaderWord = 0; boardHeaderWord < (sizeof(PhysCrateDataTileHeader) + sizeof(A2795DataBlock::Header)) / sizeof(icarus::A2795DataBlock::data_t); ++boardHeaderWord)
    {
      setA2795Word(compressed_fragment, compressedDataOffset, getA2795Word(f, uncompressedDataOffset));
      ++compressedDataOffset;
      ++uncompressedDataOffset;
    }
    // check offsets
    // since the 0th sample is used as a reference, store it as is
    compressionKeys[board*overlay.nSamplesPerChannel()] = {};
    boardDataTileSize += SampleBytesFromKey({});
    for (size_t channel = 0; channel < overlay.nChannelsPerBoard(); ++channel)
    {
      A2795DataBlock::data_t sampleADC  = (overlay.adc_val(board, channel, 0) & 0x0FFF);
      sampleADC += 0x8000;
      setA2795Word(compressed_fragment, compressedDataOffset, sampleADC);
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
        bool isComp = (std::abs(adcDiff_0) < 8) &&
                      (std::abs(adcDiff_1) < 8) &&
                      (std::abs(adcDiff_2) < 8) &&
                      (std::abs(adcDiff_3) < 8);
        uncompressedDataOffset += 4;
        // if the 4 diffs are all less than 8 in magnetude then we compress the block
        if (isComp)
        {
          compressionKeys[board*overlay.nSamplesPerChannel() + sample][channelBlock] = true;
          oddCompressions = (not oddCompressions);
          uint16_t new_word  = (adcDiff_3 & 0x000F);
          new_word <<= 4;
                   new_word += (adcDiff_2 & 0x000F);
          new_word <<= 4;
                   new_word += (adcDiff_1 & 0x000F);
          new_word <<= 4;
                   new_word += (adcDiff_0 & 0x000F);
          setA2795Word(compressed_fragment, compressedDataOffset, new_word);
          ++compressedDataOffset;
        } else {
          compressionKeys[board*overlay.nSamplesPerChannel() + sample][channelBlock] = false;
          setA2795Word(compressed_fragment, compressedDataOffset + 0, (adcDiff_0 & 0x0FFF) + 0x8000);
          setA2795Word(compressed_fragment, compressedDataOffset + 1, (adcDiff_1 & 0x0FFF) + 0x8000);
          setA2795Word(compressed_fragment, compressedDataOffset + 2, (adcDiff_2 & 0x0FFF) + 0x8000);
          setA2795Word(compressed_fragment, compressedDataOffset + 3, (adcDiff_3 & 0x0FFF) + 0x8000);
          compressedDataOffset += 4;
        }
      }
      // if there are an odd number of words in the difference then there needs to be a spacer added
      if (oddCompressions)
      {
        setA2795Word(compressed_fragment, compressedDataOffset, 0);
        ++compressedDataOffset;
      }
      // add up the size of the differences
      boardDataTileSize += SampleBytesFromKey(compressionKeys[board*overlay.nSamplesPerChannel() + sample]);
    }
    // ...and each board has a trailer
    boardDataTileSize += 4*sizeof(uint16_t);
    for (size_t boardTrailerWord = 0; boardTrailerWord < 4; ++boardTrailerWord)
    {
      setA2795Word(compressed_fragment, compressedDataOffset, getA2795Word(f, uncompressedDataOffset));
      ++compressedDataOffset;
      ++uncompressedDataOffset;
    }
    // now that we know the size for the data tile, store that in the tile header
    // endianness is weird for this, hence the htonl...
    icarus::PhysCrateDataTileHeader* boardHeader = reinterpret_cast<PhysCrateDataTileHeader*>(compressed_fragment.dataBeginBytes() + totalDataTileSize);
    boardHeader->packSize = htonl(boardDataTileSize);
    totalDataTileSize += boardDataTileSize;
  }
  
  compressed_fragment.resizeBytes(totalDataTileSize);
  
  // updated the metadata to reflect the compression
  if (compressed_fragment.hasMetadata())
    compressed_fragment.metadata<icarus::PhysCrateFragmentMetadata>()->SetCompressionScheme(1);

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
      setA2795Word(decompressed_fragment, decompressedDataOffset, getA2795Word(f, compressedDataOffset));
      ++decompressedDataOffset;
      ++compressedDataOffset;
    }
    for (size_t sample = 0; sample < overlay.nSamplesPerChannel(); ++sample)
    {
      for (size_t channel = 0; channel < overlay.nChannelsPerBoard(); ++channel)
      {
        setA2795Word(decompressed_fragment, decompressedDataOffset, overlay.adc_val(board, channel, sample));
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
      setA2795Word(decompressed_fragment, decompressedDataOffset, getA2795Word(f, compressedDataOffset));
      ++decompressedDataOffset;
      ++compressedDataOffset;
    }
    // update the board tile header packSize
    // endianness is weird for this, hence the htonl...
    icarus::PhysCrateDataTileHeader* boardHeader = reinterpret_cast<PhysCrateDataTileHeader*>(decompressed_fragment.dataBeginBytes() 
                                                                                              + board*(sizeof(PhysCrateDataTileHeader) + sizeof(A2795DataBlock::Header)) / sizeof(uint16_t)
                                                                                              + board*overlay.nChannelsPerBoard()*overlay.nSamplesPerChannel()
                                                                                              + board*4);
    boardHeader->packSize = htonl(sizeof(PhysCrateDataTileHeader) + sizeof(A2795DataBlock::Header) + overlay.nChannelsPerBoard()*overlay.nSamplesPerChannel()*sizeof(uint16_t) + 4*sizeof(uint16_t));
  }
  
  // updated the metadata to reflect the compression
  if (decompressed_fragment.hasMetadata())
    decompressed_fragment.metadata<icarus::PhysCrateFragmentMetadata>()->SetCompressionScheme(0);

  return decompressed_fragment;
}
