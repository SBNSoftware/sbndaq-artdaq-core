#include "sbndaq-artdaq-core/Overlays/SBND/PennMicroSliceWriter.hh"

sbnddaq::PennMicroSliceWriter::
PennMicroSliceWriter(uint8_t* address, uint32_t max_size_bytes) :
		     //uint8_t sequence_id) :
    PennMicroSlice(address), max_size_bytes_(max_size_bytes)
{
  header_()->format_version = 0xE1;
  //header_()->sequence_id    = sequence_id;
  header_()->block_size     = sizeof(Header);

  if (max_size_bytes_ < size()) {
    max_size_bytes_ = size();
  }
}

/*
void sbnddaq::PennMicroSliceWriter::setSequenceId(uint8_t sequence_id)
{
  header_()->sequence_id = sequence_id;
}
*/

/*
bool sbnddaq::PennMicroSliceWriter::addSample(uint16_t value)
{
  if ((size() + sizeof(value)) > max_size_bytes_) {
    return false;
  }
  data_()[header_()->sample_count] = value;
  ++(header_()->sample_count);
  header_()->nanoslice_size += sizeof(value);
  return true;
}
*/

bool sbnddaq::PennMicroSliceWriter::addData(uint16_t* data, uint16_t data_size)
{
  if ((size() + data_size) > max_size_bytes_) {
    return false;
  }
  for(uint16_t i = 0; i < data_size; i++) {
    uint16_t value = data[i];
    data_()[i] = value;
    header_()->block_size += sizeof(value);
  }
  return true;
}

int32_t sbnddaq::PennMicroSliceWriter::finalize()
{
  int32_t size_diff = max_size_bytes_ - header_()->block_size;
  max_size_bytes_ = header_()->block_size;
  return size_diff;
}

sbnddaq::PennMicroSlice::Header * sbnddaq::PennMicroSliceWriter::header_()
{
  return reinterpret_cast<Header *>(buffer_);
}

uint16_t * sbnddaq::PennMicroSliceWriter::data_()
{
  // This is not really a good solution.
  return reinterpret_cast<uint16_t *>(buffer_ + sizeof(Header));
}
