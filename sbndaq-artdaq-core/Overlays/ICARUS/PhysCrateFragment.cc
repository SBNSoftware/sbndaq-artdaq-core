#include "sbndaq-artdaq-core/Overlays/ICARUS/PhysCrateFragment.hh"
#include "cetlib/exception.h"
#include "sbndaq-artdaq-core/Trace/trace_defines.h"

#include "icarus-artdaq-base/common.h"

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
  return ( reinterpret_cast< A2795DataBlock::data_t const *>
	   (artdaq_Fragment_.dataBeginBytes() + sizeof(PhysCrateDataTileHeader)
	    + b*(sizeof(PhysCrateDataTileHeader) + 4*sizeof(uint16_t) + BoardBlockSize())
	    + sizeof(A2795DataBlock::Header)) );  
}

icarus::A2795DataBlock::data_t icarus::PhysCrateFragment::adc_val(size_t b,size_t c, size_t s) const{
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
  stop_addr      = (*(reinterpret_cast<const uint32_t*>(&(h.info1))) & 0xff00)>>16;
  mode           = (*(reinterpret_cast<const uint32_t*>(&(h.info1))) & 0x00ff);
  dead_peak_mask = (*(reinterpret_cast<const uint32_t*>(&(h.info2))));
  tv_trcr        = (*(reinterpret_cast<const uint32_t*>(&(h.info3))) & 0xff00)>>16;
  abs_time       = (*(reinterpret_cast<const uint32_t*>(&(h.info3))) & 0x00ff);
  timeinfo       = h.timeinfo;
  pkt_fmt_ver    = (*(reinterpret_cast<const uint32_t*>(&(h.chID))) & 0xf000)>>24;
  crate_id       = (*(reinterpret_cast<const uint32_t*>(&(h.chID))) & 0x0f00)>>16;
  board_id       = (*(reinterpret_cast<const uint32_t*>(&(h.chID))) & 0x00f0)>>8;
  board_status   = (*(reinterpret_cast<const uint32_t*>(&(h.chID))) & 0x000f);
  packSize       = (*(reinterpret_cast<const uint32_t*>(&(h.packSize))));

}

std::ostream & icarus::operator << (std::ostream & os, struct PhysCrateDataTileHeader const & h){
  os << "\nPhysCrateDataTileHeader: ";
  os << "\n\tToken: " << h.token;
  os << "\n\tStopAddress: " << h.stop_addr;
  os << "\n\tMode: " << h.mode;
  os << "\n\tDeadalus peak mask: " << h.dead_peak_mask;
  os << "\n\tTV TRCR: " << h.tv_trcr;
  os << "\n\tAbsolute time chunk: " << h.abs_time;
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

  if(!isCompressed()){
    TRACE(TR_LOG,"Data is compressed, so no verification done.");
    return verified;
  }

  if(artdaq_Fragment_.dataSizeBytes()!=(BoardBlockSize()*nBoards())){
    TRACEN("PhysCrateFragment",TLVL_WARNING,"PhysCrateFragment::Verify : Data size not correct!\n\tObserved=%lu, Expected=%lu\n",
	  artdaq_Fragment_.dataSizeBytes(),
	  BoardBlockSize()*nBoards());
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
