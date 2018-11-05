#include "sbndaq-artdaq-core/Overlays/SBND/PennMilliSlice.hh"
#include "sbndaq-artdaq-core/Overlays/Common/Utilities.hh"
#include <iostream>
#include <bitset>

// #define __DEBUG_payload__

sbnddaq::PennMilliSlice::PennMilliSlice(uint8_t* address) : buffer_(address), current_payload_(address), current_word_id_(0)
{
}

sbnddaq::PennMilliSlice::Header::millislice_size_t sbnddaq::PennMilliSlice::size() const
{
  return header_()->millislice_size;
}

#ifdef PENN_DONT_REBLOCK_USLICES
sbnddaq::PennMilliSlice::Header::microslice_count_t sbnddaq::PennMilliSlice::microSliceCount() const
{
  return header_()->microslice_count;
}
#endif

sbnddaq::PennMilliSlice::Header::sequence_id_t sbnddaq::PennMilliSlice::sequenceID() const
{
  return header_()->sequence_id;
}

sbnddaq::PennMilliSlice::Header::version_t sbnddaq::PennMilliSlice::version() const
{
  return header_()->version;
}

sbnddaq::PennMilliSlice::Header::timestamp_t sbnddaq::PennMilliSlice::endTimestamp() const
{
  return header_()->end_timestamp;
}

sbnddaq::PennMilliSlice::Header::ticks_t sbnddaq::PennMilliSlice::widthTicks() const
{
  return header_()->width_in_ticks;
}

sbnddaq::PennMilliSlice::Header::ticks_t sbnddaq::PennMilliSlice::overlapTicks() const
{
  return header_()->overlap_in_ticks;
}

sbnddaq::PennMilliSlice::Header::payload_count_t sbnddaq::PennMilliSlice::payloadCount() const
{
  return header_()->payload_count;
}

sbnddaq::PennMilliSlice::Header::payload_count_t sbnddaq::PennMilliSlice::payloadCount(
										 sbnddaq::PennMilliSlice::Header::payload_count_t& counter, 
										 sbnddaq::PennMilliSlice::Header::payload_count_t& trigger,
										 sbnddaq::PennMilliSlice::Header::payload_count_t& timestamp) const
{
  counter   = header_()->payload_count_counter;
  trigger   = header_()->payload_count_trigger;
  timestamp = header_()->payload_count_timestamp;
  return payloadCount();
}

#ifdef PENN_DONT_REBLOCK_USLICES
// Returns the requested MicroSlice if the requested slice was found,
// otherwise returns an empty pointer
std::unique_ptr<sbnddaq::PennMicroSlice> sbnddaq::PennMilliSlice::microSlice(uint32_t index) const
{
  std::unique_ptr<PennMicroSlice> mslice_ptr;
  if (index < microSliceCount()) {
    uint8_t* ms_ptr = data_(index);
    mslice_ptr.reset(new PennMicroSlice(ms_ptr));
  }
  return mslice_ptr;
}
#endif

//Returns the requested payload
uint8_t* sbnddaq::PennMilliSlice::get_next_payload(uint32_t& index, 
						sbnddaq::PennMicroSlice::Payload_Header::data_packet_type_t& data_packet_type,
						sbnddaq::PennMicroSlice::Payload_Header::short_nova_timestamp_t& short_nova_timestamp,
						size_t& payload_size) 
{

  if(current_word_id_ == 0){
    current_payload_ = buffer_ + sizeof(Header);
  }
  else{
    //current_payload_ points to the last payload served up
    //We need to skip over this one
    sbnddaq::PennMicroSlice::Payload_Header* payload_header = sbndaq::reinterpret_cast_checked<sbnddaq::PennMicroSlice::Payload_Header*>(current_payload_);
    sbnddaq::PennMicroSlice::Payload_Header::data_packet_type_t type = payload_header->data_packet_type;
    switch(type)
      {
      case sbnddaq::PennMicroSlice::DataTypeCounter:
	current_payload_ += sbnddaq::PennMicroSlice::Payload_Header::size_bytes + sbnddaq::PennMicroSlice::payload_size_counter;
	break;
      case sbnddaq::PennMicroSlice::DataTypeTrigger:
	current_payload_ += sbnddaq::PennMicroSlice::Payload_Header::size_bytes + sbnddaq::PennMicroSlice::payload_size_trigger;
	break;
      case sbnddaq::PennMicroSlice::DataTypeTimestamp:
	current_payload_ += sbnddaq::PennMicroSlice::Payload_Header::size_bytes + sbnddaq::PennMicroSlice::payload_size_timestamp;
	break;
      case sbnddaq::PennMicroSlice::DataTypeWarning:
	current_payload_ += sbnddaq::PennMicroSlice::Payload_Header::size_bytes + sbnddaq::PennMicroSlice::payload_size_warning;
	break;
      case sbnddaq::PennMicroSlice::DataTypeChecksum:
	current_payload_ += sbnddaq::PennMicroSlice::Payload_Header::size_bytes + sbnddaq::PennMicroSlice::payload_size_checksum;
	break;
      default:
//	std::cerr << "Unknown data packet type found 0x" << std::hex << (unsigned int)type << std::endl;
	return nullptr;
	break;
      }//switch(type)
  }//(current_word_id_ != 0)

  //Now current_payload_ will point to the next payload. Process the information that we want

  //Need to make sure we have not gone off the end of the buffer
  if(current_payload_ >= (buffer_ + size())) 
    return nullptr;
  

  sbnddaq::PennMicroSlice::Payload_Header* payload_header = sbndaq::reinterpret_cast_checked<sbnddaq::PennMicroSlice::Payload_Header*>(current_payload_);
  sbnddaq::PennMicroSlice::Payload_Header::data_packet_type_t type = payload_header->data_packet_type;

  //Set the variables passed as references
  index = current_word_id_;
  type = payload_header->data_packet_type;
  data_packet_type = type;
  short_nova_timestamp = payload_header->short_nova_timestamp;

  switch(type)
    {
    case sbnddaq::PennMicroSlice::DataTypeCounter:
      payload_size = sbnddaq::PennMicroSlice::payload_size_counter;
      break;
    case sbnddaq::PennMicroSlice::DataTypeTrigger:
      payload_size = sbnddaq::PennMicroSlice::payload_size_trigger;
      break;
    case sbnddaq::PennMicroSlice::DataTypeTimestamp:
      payload_size = sbnddaq::PennMicroSlice::payload_size_timestamp;
      break;
    case sbnddaq::PennMicroSlice::DataTypeWarning:
      payload_size = sbnddaq::PennMicroSlice::payload_size_warning;
      break;
    case sbnddaq::PennMicroSlice::DataTypeChecksum:
      payload_size = sbnddaq::PennMicroSlice::payload_size_checksum;
      break;
    default:
      //std::cerr << "Unknown data packet type found 0x" << std::hex << (unsigned int)type << std::endl;
      payload_size = 0;
      break;
    }//switch(type)

  current_word_id_++;
  //current_payload_ points at the Payload_Header of the next payload
  //this function assumes that state is true the next time it is called (so it knows how to shift to the next payload)
  //but the user is expecting just the payload data. Therefore return the buffer offset by the payload_header

  return (current_payload_ + sbnddaq::PennMicroSlice::Payload_Header::size_bytes);

}


uint8_t* sbnddaq::PennMilliSlice::get_next_payload(uint32_t &index,sbnddaq::PennMicroSlice::Payload_Header*& data_header) {

  // If we are still at the beginning, shift to the beginning of the Millislice and
  // jump over its header
  if(current_word_id_ == 0) {
    current_payload_ = buffer_ + sizeof(Header);
  }
  else{
    //current_payload_ points to the last payload served
    //We need to skip over this one
    sbnddaq::PennMicroSlice::Payload_Header* payload_header = sbndaq::reinterpret_cast_checked<sbnddaq::PennMicroSlice::Payload_Header*>(current_payload_);
    sbnddaq::PennMicroSlice::Payload_Header::data_packet_type_t type = payload_header->data_packet_type;
    switch(type)
      {
      case sbnddaq::PennMicroSlice::DataTypeCounter:
        current_payload_ += sbnddaq::PennMicroSlice::Payload_Header::size_bytes + sbnddaq::PennMicroSlice::payload_size_counter;
        break;
      case sbnddaq::PennMicroSlice::DataTypeTrigger:
        current_payload_ += sbnddaq::PennMicroSlice::Payload_Header::size_bytes + sbnddaq::PennMicroSlice::payload_size_trigger;
        break;
      case sbnddaq::PennMicroSlice::DataTypeTimestamp:
        current_payload_ += sbnddaq::PennMicroSlice::Payload_Header::size_bytes + sbnddaq::PennMicroSlice::payload_size_timestamp;
        break;
      case sbnddaq::PennMicroSlice::DataTypeWarning:
        current_payload_ += sbnddaq::PennMicroSlice::Payload_Header::size_bytes + sbnddaq::PennMicroSlice::payload_size_warning;
        break;
      case sbnddaq::PennMicroSlice::DataTypeChecksum:
        current_payload_ += sbnddaq::PennMicroSlice::Payload_Header::size_bytes + sbnddaq::PennMicroSlice::payload_size_checksum;
        break;
      default:
        //std::cerr << "Unknown data packet type found 0x" << std::hex << (unsigned int)type << std::endl;
        return nullptr;
        break;
      }//switch(type)
  }//(current_word_id_ != 0)

  //Now current_payload_ will point to the next payload. Process the information that we want

  //Need to make sure we have not gone off the end of the buffer
  if(current_payload_ >= (buffer_ + size()))
    return nullptr;


  sbnddaq::PennMicroSlice::Payload_Header* payload_header = sbndaq::reinterpret_cast_checked<sbnddaq::PennMicroSlice::Payload_Header*>(current_payload_);
  //sbnddaq::PennMicroSlice::Payload_Header::data_packet_type_t type = payload_header->data_packet_type;
  
  //Set the variables passed as references
  index = current_word_id_;
  //type = payload_header->data_packet_type;
  data_header = payload_header;

  current_word_id_++;
  //current_payload_ points at the Payload_Header of the next payload
  //this function assumes that state is true the next time it is called (so it knows how to shift to the next payload)
  //but the user is expecting just the payload data. Therefore return the buffer offset by the payload_header

  return (current_payload_ + sbnddaq::PennMicroSlice::Payload_Header::size_bytes);

}

// Returns the nearest timestamp word in the future
// does not change internal pointers
uint8_t* sbnddaq::PennMilliSlice::get_next_timestamp(sbnddaq::PennMicroSlice::Payload_Header*& data_header) {
  uint8_t* pl_ptr = current_payload_;
  bool found = false;
  if(current_word_id_ == 0) {
    pl_ptr = buffer_ + sizeof(Header);
  }
  sbnddaq::PennMicroSlice::Payload_Header* payload_header = nullptr;
  while (!found && (pl_ptr < (buffer_ + size()))) {
    //current_payload_ points to the last payload served
    //We need to skip over this one
    payload_header = sbndaq::reinterpret_cast_checked<sbnddaq::PennMicroSlice::Payload_Header*>(pl_ptr);
    sbnddaq::PennMicroSlice::Payload_Header::data_packet_type_t type = payload_header->data_packet_type;
    switch(type)
      {
      case sbnddaq::PennMicroSlice::DataTypeCounter:
        pl_ptr += sbnddaq::PennMicroSlice::Payload_Header::size_bytes + sbnddaq::PennMicroSlice::payload_size_counter;
        break;
      case sbnddaq::PennMicroSlice::DataTypeTrigger:
        pl_ptr += sbnddaq::PennMicroSlice::Payload_Header::size_bytes + sbnddaq::PennMicroSlice::payload_size_trigger;
        break;
      case sbnddaq::PennMicroSlice::DataTypeTimestamp:
          found = true;
          break;
      case sbnddaq::PennMicroSlice::DataTypeWarning:
        pl_ptr += sbnddaq::PennMicroSlice::Payload_Header::size_bytes + sbnddaq::PennMicroSlice::payload_size_warning;
        break;
      case sbnddaq::PennMicroSlice::DataTypeChecksum:
        pl_ptr += sbnddaq::PennMicroSlice::Payload_Header::size_bytes + sbnddaq::PennMicroSlice::payload_size_checksum;
        break;
      default:
        //std::cerr << "Unknown data packet type found 0x" << std::hex << (unsigned int)type << std::endl;
        data_header = nullptr;
        return nullptr;
        break;
      }//switch(type)
  }

  if (found) {
    data_header = sbndaq::reinterpret_cast_checked<sbnddaq::PennMicroSlice::Payload_Header*>(pl_ptr);
    return (pl_ptr + sbnddaq::PennMicroSlice::Payload_Header::size_bytes);
  }
  else return nullptr;
}

//Returns the requested payload
uint8_t* sbnddaq::PennMilliSlice::payload(uint32_t index, 
				       sbnddaq::PennMicroSlice::Payload_Header*& data_header) const
{
  uint32_t i = 0;
  uint8_t* pl_ptr = buffer_ + sizeof(Header);
  while(pl_ptr < (buffer_ + size())) {
    sbnddaq::PennMicroSlice::Payload_Header* payload_header = reinterpret_cast<sbnddaq::PennMicroSlice::Payload_Header*>(pl_ptr);
    sbnddaq::PennMicroSlice::Payload_Header::data_packet_type_t type = payload_header->data_packet_type;
    if(i == index) {
      data_header = payload_header;
      pl_ptr += sbnddaq::PennMicroSlice::Payload_Header::size_bytes;
      return pl_ptr;
    }
    switch(type) {
      case sbnddaq::PennMicroSlice::DataTypeCounter:
        pl_ptr += sbnddaq::PennMicroSlice::payload_size_counter;
        break;
      case sbnddaq::PennMicroSlice::DataTypeTrigger:
        pl_ptr += sbnddaq::PennMicroSlice::payload_size_trigger;
        break;
      case sbnddaq::PennMicroSlice::DataTypeTimestamp:
        pl_ptr += sbnddaq::PennMicroSlice::payload_size_timestamp;
        break;
      case sbnddaq::PennMicroSlice::DataTypeWarning:
        pl_ptr += sbnddaq::PennMicroSlice::payload_size_warning;
        break;
      case sbnddaq::PennMicroSlice::DataTypeChecksum:
        pl_ptr += sbnddaq::PennMicroSlice::payload_size_checksum;
        break;
      default:
        //std::cerr << "Unknown data packet type found 0x" << std::hex << (unsigned int)type << std::endl;
        return nullptr;
        break;
      }//switch(type)
    ++i;
  }
  std::cerr << "Could not find payload with index " << index << " (the data buffer has overrun)" << std::endl;
  return nullptr;
}

//Returns the requested payload
uint8_t* sbnddaq::PennMilliSlice::payload(uint32_t index,
               sbnddaq::PennMicroSlice::Payload_Header::data_packet_type_t& data_packet_type,
               sbnddaq::PennMicroSlice::Payload_Header::short_nova_timestamp_t& short_nova_timestamp,
               size_t& payload_size) const
{
  uint32_t i = 0;
  uint8_t* pl_ptr = buffer_ + sizeof(Header);
  while(pl_ptr < (buffer_ + size())) {
    sbnddaq::PennMicroSlice::Payload_Header* payload_header = reinterpret_cast<sbnddaq::PennMicroSlice::Payload_Header*>(pl_ptr);
    sbnddaq::PennMicroSlice::Payload_Header::data_packet_type_t type = payload_header->data_packet_type;
    if(i == index) {
      data_packet_type = type;
      short_nova_timestamp = payload_header->short_nova_timestamp;
#ifdef __DEBUG_payload__
    std::cout << "PennMilliSlice::payload() payload  " << i << " has type 0x"
        << std::hex << (unsigned int)data_packet_type << std::dec
        << " " << std::bitset<3>(data_packet_type)
        << " and timestamp " << short_nova_timestamp
        << " " << std::bitset<28>(short_nova_timestamp)
        << " payload header bits " << std::bitset<32>(*((uint32_t*)pl_ptr))
        << std::endl;
#endif
      pl_ptr += sbnddaq::PennMicroSlice::Payload_Header::size_bytes;
      switch(type)
  {
  case sbnddaq::PennMicroSlice::DataTypeCounter:
    payload_size = sbnddaq::PennMicroSlice::payload_size_counter;
    break;
  case sbnddaq::PennMicroSlice::DataTypeTrigger:
    payload_size = sbnddaq::PennMicroSlice::payload_size_trigger;
    break;
  case sbnddaq::PennMicroSlice::DataTypeTimestamp:
    payload_size = sbnddaq::PennMicroSlice::payload_size_timestamp;
    break;
  case sbnddaq::PennMicroSlice::DataTypeWarning:
    payload_size = sbnddaq::PennMicroSlice::payload_size_warning;
    break;
  case sbnddaq::PennMicroSlice::DataTypeChecksum:
    payload_size = sbnddaq::PennMicroSlice::payload_size_checksum;
    break;
  default:
    //std::cerr << "Unknown data packet type found 0x" << std::hex << (unsigned int)type << std::endl;
    payload_size = 0;
    break;
  }//switch(type)
      return pl_ptr;
    }
    switch(type)
      {
      case sbnddaq::PennMicroSlice::DataTypeCounter:
  pl_ptr += sbnddaq::PennMicroSlice::Payload_Header::size_bytes + sbnddaq::PennMicroSlice::payload_size_counter;
  break;
      case sbnddaq::PennMicroSlice::DataTypeTrigger:
  pl_ptr += sbnddaq::PennMicroSlice::Payload_Header::size_bytes + sbnddaq::PennMicroSlice::payload_size_trigger;
  break;
      case sbnddaq::PennMicroSlice::DataTypeTimestamp:
  pl_ptr += sbnddaq::PennMicroSlice::Payload_Header::size_bytes + sbnddaq::PennMicroSlice::payload_size_timestamp;
  break;
      case sbnddaq::PennMicroSlice::DataTypeWarning:
  pl_ptr += sbnddaq::PennMicroSlice::Payload_Header::size_bytes + sbnddaq::PennMicroSlice::payload_size_warning;
  break;
      case sbnddaq::PennMicroSlice::DataTypeChecksum:
  pl_ptr += sbnddaq::PennMicroSlice::Payload_Header::size_bytes + sbnddaq::PennMicroSlice::payload_size_checksum;
  break;
      default:
  //std::cerr << "Unknown data packet type found 0x" << std::hex << (unsigned int)type << std::endl;
  return nullptr;
  break;
      }//switch(type)
    i++;
  }
  std::cerr << "Could not find payload with ID " << index << " (the data buffer has overrun)" << std::endl;
  payload_size = 0;
  return nullptr;
}

#ifdef ENABLE_PENNMILLISLICE_CHECKSUM

sbnddaq::PennMilliSlice::checksum_t sbnddaq::PennMilliSlice::calculateChecksum() const
{
  try {
    //NFB: Is the checksum actually used anywhere? It certainly complicates the calculations.
    boost::crc_32_type checksum;
    checksum.process_bytes(buffer_, this->size());
    return checksum.checksum();
  }
  catch ( ... ) {
    std::cout << "Error caught in PennMilliSlice::calculateChecksum()" << std::endl;
    //TODO handle error cleanly here
    return 0;
  }
}

sbnddaq::PennMilliSlice::checksum_t sbnddaq::PennMilliSlice::checksum() const
{
  return *(reinterpret_cast<sbnddaq::PennMilliSlice::checksum_t*>(buffer_ + this->size() - sizeof(sbnddaq::PennMilliSlice::checksum_t)));
}
#endif

sbnddaq::PennMilliSlice::Header const* sbnddaq::PennMilliSlice::header_() const
{
  return reinterpret_cast<Header const*>(buffer_);
}

// returns a pointer to the requested MicroSlice
uint8_t* sbnddaq::PennMilliSlice::data_(int index) const
{
  uint8_t* ms_ptr = buffer_ + sizeof(Header);
  for (int idx = 0; idx < index; ++idx) {
    PennMicroSlice tmp_ms(ms_ptr);
    ms_ptr += tmp_ms.size();
  }
  return ms_ptr;
}
