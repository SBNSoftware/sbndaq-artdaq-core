#ifndef sbndaq_artdaq_core_Overlays_ICARUS_PhysCrateFragment_hh
#define sbndaq_artdaq_core_Overlays_ICARUS_PhysCrateFragment_hh

#include "artdaq-core/Data/Fragment.hh"
#include "cetlib_except/exception.h"

#include <bitset>
#include <iostream>
#include <vector>
#include <array>

#include "sbndaq-artdaq-core/Overlays/ICARUS/structures.h"

// Implementation of "PhysCrateFragment", an artdaq::Fragment overlay class
// used for ICARUS DAQ

namespace icarus {
  class PhysCrateFragment;
  std::ostream & operator << (std::ostream &, PhysCrateFragment const &);

  class PhysCrateFragmentMetadata;
  std::ostream & operator << (std::ostream &, PhysCrateFragmentMetadata const&);

  struct PhysCrateDataTileHeader;
  std::ostream & operator << (std::ostream &, struct PhysCrateDataTileHeader const&);

  struct A2795DataBlock;
  std::ostream & operator << (std::ostream &, struct A2795DataBlock const&);
}

//same structure that is found in structures.h
struct icarus::PhysCrateDataTileHeader{
  
  //'DATA'
  uint32_t token;
  
  //info1
  //this is an update based on looking at info from May2019
  //uint32_t stop_addr : 16;
  //uint32_t mode      : 16;
  //this is an update based on looking at info from May2019

  uint32_t info1;
  /*
  uint32_t ctrl_bufforg  : 4;
  uint32_t ctrl_status   : 4; //(CTRL_INHIBIT_TRG=0x8 | CTRL_ACQRUN=0x1)
  uint32_t ctrl_datamode : 4; //(CTRL_TTL_PROPAG=0x4 | CTRL_SRAM_TEST=0x2 | CTRL_TEST_PATTERN=0x1)
  uint32_t ctrl_unused1  : 4;
  uint32_t ctrl_unused2  : 4;
  uint32_t ctrl_unused3  : 4;
  uint32_t ctrl_unused4  : 4;
  uint32_t ctrl_testmode : 4; //( CTRL_TEST_EVENT=0x8 | CTRL_TEST_MODE=0x1)
  */

  //info2
  //uint32_t dead_peak_mask;
  //this is an update based on looking at info from May2019

  uint32_t info2;
  /*
  uint32_t slot_id : 4;
  uint32_t status  : 4; // (GBUSY=0x8 | BUSY=0x4 | DRDY=0x2 | RUNNING=0x1)
  uint32_t unused1 : 24;
  */

  //info3
  //uint32_t tv_trcr  : 16;
  //uint32_t abs_time : 16;
  //this is an update based on looking at info from May2019
  
  uint32_t info3;
  //uint32_t nev_stored;

  //timeinfo
  uint32_t timeinfo;
  
  //chID
  uint32_t pkt_fmt_ver  : 8;
  uint32_t crate_id     : 8;
  uint32_t board_id     : 8;
  uint32_t board_status : 8;
  
  //packSize
  uint32_t packSize;

  PhysCrateDataTileHeader(){};
  PhysCrateDataTileHeader( struct DataTile::Header const& );


  uint32_t CtrlStatus_BuffOrg()     const { return (info1 & 0x0000000F); }
  uint32_t CtrlStatus_AcqRun()      const { return (info1 & 0x00000010) >> 4; }
  uint32_t CtrlStatus_InhibitTrg()  const { return (info1 & 0x00000080) >> 4; }
  uint32_t CtrlStatus_TestPattern() const { return (info1 & 0x00000100) >> 8; }
  uint32_t CtrlStatus_SRAMTest()    const { return (info1 & 0x00000200) >> 9; }
  uint32_t CtrlStatus_TTLPropag()   const { return (info1 & 0x00000400) >> 10; }
  uint32_t CtrlStatus_TestMode()    const { return (info1 & 0x10000000) >> 28; }
  uint32_t CtrlStatus_TestEvent()   const { return (info1 & 0x80000000) >> 31; }  

  uint32_t StatusReg_SlotID()       const { return (info2 & 0x0000000F); }
  uint32_t StatusReg_Running()      const { return (info2 & 0x00000010) >> 4; }
  uint32_t StatusReg_DataReady()    const { return (info2 & 0x00000020) >> 5; }
  uint32_t StatusReg_Busy()         const { return (info2 & 0x00000040) >> 6; }
  uint32_t StatusReg_GBusy()        const { return (info2 & 0x00000080) >> 7; }


};
struct icarus::A2795DataBlock{

  typedef uint32_t header_t;
  typedef uint16_t data_t;

  typedef struct {
    header_t event_number : 24;
    header_t unused1      :  8;
    header_t time_stamp;
  } Header;

  Header  header;
  data_t* data;
};

class icarus::PhysCrateFragmentMetadata {
  
public:
  
  typedef uint32_t data_t; //fundamental unit of metadata
  typedef data_t   id_t; //type for board IDs
  
  PhysCrateFragmentMetadata(){}
  PhysCrateFragmentMetadata(data_t run_number,
			    data_t n_boards,
			    data_t channels_per_board,
			    data_t samples_per_channel,
			    data_t adcs_per_sample,
			    data_t compression,
			   std::vector<id_t> const& idvec)
  { 
    _run_number = run_number;
    _num_boards = n_boards;
    _channels_per_board = channels_per_board;
    _samples_per_channel = samples_per_channel;
    _num_adc_bits = adcs_per_sample;
    _compression_scheme = compression;
    SetBoardIDs(idvec);
  }

  data_t const& run_number() const { return _run_number; }
  data_t const& samples_per_channel() const { return _samples_per_channel; }
  data_t const& num_adc_bits() const { return _num_adc_bits; }
  data_t const& channels_per_board() const { return _channels_per_board; }
  data_t const& num_boards() const { return _num_boards; }
  data_t const& compression_scheme() const { return _compression_scheme; }

  id_t   const& board_id(size_t i) const
  { BoardExists(i); return _board_ids[i]; }

  void  SetBoardID(size_t i,id_t id)
  { BoardExists(i); _board_ids[i] = id; }
  void  SetBoardIDs(std::vector<id_t> const& idvec)
  { CheckNBoards(idvec.size()); _board_ids = idvec; } 
  void  SetCompressionScheme(data_t scheme)
  { _compression_scheme = scheme; }

  void BoardExists(size_t i) const;
  void CheckNBoards(size_t i) const;

  size_t ExpectedDataSize() const 
  { return _num_boards*(sizeof(PhysCrateDataTileHeader) +
			sizeof(A2795DataBlock::Header) +
			_channels_per_board*_samples_per_channel*sizeof(A2795DataBlock::data_t) +
			2*sizeof(uint32_t)); }


private:

  data_t _run_number;
  data_t _samples_per_channel;  
  data_t _num_adc_bits;
  data_t _channels_per_board;
  data_t _num_boards;
  data_t _compression_scheme;
  std::vector<id_t> _board_ids;
  
  void UpdateBoardIDSize(){ _board_ids.resize(_num_boards); }

};

class icarus::PhysCrateFragment {

  public:

  PhysCrateFragment(artdaq::Fragment const & f) : artdaq_Fragment_(f),
                                                  accessors_(f.metadata<icarus::PhysCrateFragmentMetadata>()->num_boards(),
                                                             f.metadata<icarus::PhysCrateFragmentMetadata>()->channels_per_board(),
                                                             f.metadata<icarus::PhysCrateFragmentMetadata>()->samples_per_channel())
                                                  {
                                                    accessors_ = icarus::PhysCrateFragment::GenerateAccessors(artdaq_Fragment_);
                                                  }

  PhysCrateFragment(artdaq::Fragment const & f, bool const & compressionSwitch)
    : PhysCrateFragment(this->fragmentSwitch(f, compressionSwitch)) {}

  PhysCrateFragmentMetadata const * metadata() const { return artdaq_Fragment_.metadata<PhysCrateFragmentMetadata>(); }

  size_t RunNumber() const { return metadata()->run_number(); }
  size_t nBoards() const { return metadata()->num_boards(); }
  size_t nChannels() const { return metadata()->num_boards()*metadata()->channels_per_board(); }
  size_t nSamplesPerChannel() const { return metadata()->samples_per_channel(); }
  size_t nChannelsPerBoard() const { return metadata()->channels_per_board(); }
  size_t CompressionScheme() const { return metadata()->compression_scheme(); }

  bool   isCompressed() const { return (CompressionScheme()!=0); }

  size_t DataPayloadSize() const { return artdaq_Fragment_.dataSizeBytes(); }

  PhysCrateDataTileHeader const * DataTileHeader(uint16_t b=0) const;
  size_t DataTileHeaderLocation(uint16_t b=0) const;

  size_t BoardBlockSize() const
  { return sizeof(A2795DataBlock::Header)+nChannelsPerBoard()*nSamplesPerChannel()*sizeof(A2795DataBlock::data_t); }
  size_t BoardBlockSize(size_t b)
  {
    size_t boardBlockSize = sizeof(A2795DataBlock::Header);
    for (size_t s = 0; s < nSamplesPerChannel(); ++s)
      boardBlockSize += SampleBytesFromKey(CompressionKey(b, s));
    return boardBlockSize;
  }

  A2795DataBlock           const* BoardDataBlock(uint16_t b=0) const;
  A2795DataBlock::Header   const& BoardHeader(uint16_t b=0) const;
  A2795DataBlock::header_t        BoardEventNumber(uint16_t b=0) const;
  A2795DataBlock::header_t        BoardTimeStamp(uint16_t b=0) const;
  A2795DataBlock::data_t   const* BoardData(uint16_t b=0) const;

  A2795DataBlock::data_t adc_val(size_t b,size_t c,size_t s) const;
  A2795DataBlock::data_t adc_val_usingIterator(size_t b,size_t c,size_t s) const;
  std::vector<A2795DataBlock::data_t> channel_adc_vec(size_t b,size_t c) const
  {
    std::vector<A2795DataBlock::data_t>::const_iterator beginItr = accessors_.accessPair_.second.begin() + b*this->nSamplesPerChannel()*this->nChannelsPerBoard() + c*this->nSamplesPerChannel();
    std::vector<A2795DataBlock::data_t>::const_iterator backItr = beginItr + this->nSamplesPerChannel();
    std::vector<A2795DataBlock::data_t> chADCs(beginItr, backItr);
    return chADCs;
  }

  bool Verify() const;

  typedef std::array<bool, 16> Key; 
  Key const& CompressionKey(size_t b, size_t s) const
  {
    size_t index = b*this->nSamplesPerChannel() + s;
    return accessors_.accessPair_.first.at(index);
  }
  A2795DataBlock::data_t const& adcVal_fromAccessor(size_t b, size_t c, size_t s) const
  {
    // adc values are stored such that they can be easily sliced by board/channel
    size_t index = b*this->nSamplesPerChannel()*this->nChannelsPerBoard() + c*this->nSamplesPerChannel() + s;
    return accessors_.accessPair_.second.at(index);
  }

  static artdaq::Fragment   compressArtdaqFragment(artdaq::Fragment const & f);
  static artdaq::Fragment decompressArtdaqFragment(artdaq::Fragment const & f);
  static artdaq::Fragment fragmentSwitch(artdaq::Fragment const & f, bool const & compressionSwitch)
  {
    return (compressionSwitch) ? compressArtdaqFragment(f) : decompressArtdaqFragment(f);
  }

  PhysCrateFragment makeCompressedFragment()   const { return PhysCrateFragment(  compressArtdaqFragment(artdaq_Fragment_)); }
  PhysCrateFragment makeUncompressedFragment() const { return PhysCrateFragment(decompressArtdaqFragment(artdaq_Fragment_)); }

  static uint64_t getFragmentWord(artdaq::Fragment const& f, size_t nWord) 
  {
    // this returns the nth 64-bit word of the _payload_
    // note this skips Fragment headers/metadata
    // but _doesn't_ skip the board header(s) which are inside the Fragment payload
    if (nWord > f.dataSize())
      throw cet::exception("PhysCrateFragment:getFragmentWord")
        << "Asked for 64-bit word " << nWord << " of the fragment, but there are only " << f.dataSize() << " in the payload" << '\n';

    return *(f.dataBegin() + nWord);
  }

  static uint16_t getA2795Word(artdaq::Fragment const& f, size_t nWord)
  {
    // this returns the nth 16-bit word of the _payload_
    size_t nFragmentWord = nWord / 4;
    size_t shift = 16*(nWord % 4);
    uint64_t longWord = getFragmentWord(f, nFragmentWord);
    uint16_t shortWord = (longWord >> shift) & 0xFFFF;
    return shortWord;
  }

  static size_t SampleBytesFromKey(Key const& key)
  {
    size_t nCompressed = 0;
    for (auto const& bit : key)
      nCompressed += bit;

    return 128 - 6*nCompressed + 2*(nCompressed % 2);
  }

private:

  artdaq::Fragment const & artdaq_Fragment_;

  void   throwIfCompressed() const;

  // here are things helpful for the comrpessed fragments
  struct Accessors
  {
    Accessors(size_t nBoards, size_t nChannels, size_t nSamples)
    {
      accessPair_.first  = std::vector<Key>(nBoards*nSamples);
      accessPair_.second = std::vector<A2795DataBlock::data_t>(nBoards*nChannels*nSamples, 0);
    };
    Accessors(std::vector<Key> accessorKeys, std::vector<A2795DataBlock::data_t> accessorADC)
    {
      accessPair_.first  = accessorKeys;
      accessPair_.second = accessorADC;
    }
    Accessors(std::pair<std::vector<Key>, std::vector<A2795DataBlock::data_t>> accessPair)
    {
      accessPair_.first  = accessPair.first;
      accessPair_.second = accessPair.second;
    }
    Accessors(artdaq::Fragment const& f) : accessPair_(GenerateAccessors(f)) {};
    std::pair<std::vector<Key>, std::vector<A2795DataBlock::data_t>> accessPair_;
  } accessors_;

  size_t cumulativeSampleSize(size_t b, size_t s, size_t runningTotal = 0) const
  {
    // tail recursive function to total the bytes used for each sample
    // requires the compression keys to function
    Key const& key = this->CompressionKey(b, s);

    if (s == 0)
      return runningTotal + this->SampleBytesFromKey(key);

    return this->cumulativeSampleSize(b, s - 1, runningTotal + this->SampleBytesFromKey(key));
  }

  size_t cumulativeBoardSize(size_t b, size_t runningTotal = 0) const
  {
    // tail recursive function to total the bytes used for each board
    // each board is made up of nSamples samples
    // requires the compression keys to function
    size_t nSamples = this->nSamplesPerChannel();
    
    if (b == 0)
      return this->cumulativeSampleSize(0, nSamples - 1, runningTotal);

    return this->cumulativeBoardSize(b - 1, this->cumulativeSampleSize(b, nSamples - 1, runningTotal));
  }

  static std::pair<std::vector<Key>, std::vector<A2795DataBlock::data_t>> GenerateAccessors(artdaq::Fragment const& f);

  static void setFragmentWord(artdaq::Fragment& f, size_t nWord, uint64_t value)
  {
    *(f.dataBegin() + nWord) = value;
  }

  static void setA2795Word(artdaq::Fragment& f, size_t nWord, uint16_t value)
  {
    uint64_t value64 = value;
    size_t nFragmentWord = nWord / 4;
    size_t shift = 16*(nWord % 4);
    uint64_t oldWord = getFragmentWord(f, nFragmentWord);
    uint64_t filterBlock = 0xFFFF;
    uint64_t filter = ~(filterBlock << shift);
    uint64_t newWord = (oldWord & filter) + ((value64 << shift) & ~filter);
    setFragmentWord(f, nWord / 4, newWord);
  }

};


#endif /* sbndaq_artdaq_core_Overlays_ToyFragment_hh */
