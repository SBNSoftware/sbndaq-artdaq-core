#include "sbndaq-artdaq-core/Overlays/SBND/PennMilliSliceWriter.hh"

sbnddaq::PennMilliSliceWriter::
PennMilliSliceWriter(uint8_t* address, uint32_t max_size_bytes) :
    PennMilliSlice(address), max_size_bytes_(max_size_bytes)
{
  header_()->sequence_id             = 0;
  header_()->version                 = 0xFFFF;
  header_()->millislice_size         = sizeof(Header);
  header_()->payload_count           = 0;
  header_()->payload_count_counter   = 0;
  header_()->payload_count_trigger   = 0;
  header_()->payload_count_timestamp = 0;
  header_()->end_timestamp           = 0;
  header_()->width_in_ticks          = 0;
  header_()->overlap_in_ticks        = 0;
}

#ifdef PENN_DONT_REBLOCK_USLICES
std::shared_ptr<sbnddaq::PennMicroSliceWriter>
sbnddaq::PennMilliSliceWriter::reserveMicroSlice(uint32_t ms_max_bytes)
{
  // finalize the most recent PennMicroSlice, in case that hasn't
  // already been done
  finalizeLatestMicroSlice_();

  // test if this new PennMicroSlice could overflow our maximum size
  if ((size() + ms_max_bytes) > max_size_bytes_) {
    return false;
  }

  // create the next PennMicroSlice in our buffer, and update our
  // counters to include the new PennMicroSlice
  uint8_t* ms_ptr = data_(header_()->microslice_count);
  latest_microslice_ptr_.reset(new PennMicroSliceWriter(ms_ptr, ms_max_bytes));
  ++(header_()->microslice_count);
  header_()->millislice_size += ms_max_bytes;
  return latest_microslice_ptr_;
}
#endif

int32_t sbnddaq::PennMilliSliceWriter::finalize(bool override, uint32_t data_size_bytes,
#ifdef PENN_DONT_REBLOCK_USLICES
					     uint32_t microslice_count,
#endif
					     uint16_t sequence_id,
					     uint16_t payload_count, uint16_t payload_count_counter,
					     uint16_t payload_count_trigger, uint16_t payload_count_timestamp,
					     uint64_t end_timestamp, uint32_t width_in_ticks, uint32_t overlap_in_ticks)
{
  // first, we need to finalize the last MicroSlice, in case that
  // hasn't already been done
  finalizeLatestMicroSlice_();

  //Override the current size if requested
  if(override) {
    header_()->millislice_size = sizeof(Header) + data_size_bytes;
#ifdef PENN_DONT_REBLOCK_USLICES
    header_()->microslice_count = microslice_count;
#endif
    header_()->payload_count           = payload_count;
    header_()->payload_count_counter   = payload_count_counter;
    header_()->payload_count_trigger   = payload_count_trigger;
    header_()->payload_count_timestamp = payload_count_timestamp;
    header_()->end_timestamp           = end_timestamp;
    header_()->width_in_ticks          = width_in_ticks;
    header_()->overlap_in_ticks        = overlap_in_ticks;
    header_()->sequence_id             = sequence_id;
  }

#ifdef  ENABLE_PENNMILLISLICE_CHECKSUM
  //Calculate checksum
  //TODO decide if a footer is the right place to put the checksum. Alternative is associate it with the fragment
  uint8_t* end_ptr = buffer_ + header_()->millislice_size;
  *(reinterpret_cast<sbnddaq::PennMilliSliceWriter::checksum_t *>(end_ptr)) = calculateChecksum();
  header_()->millislice_size += sizeof(sbnddaq::PennMilliSliceWriter::checksum_t);
#endif
  // next, we update our maximum size so that no more MicroSlices
  // can be added
  int32_t size_diff = max_size_bytes_ - header_()->millislice_size;
  max_size_bytes_ = header_()->millislice_size;
  return size_diff;
}

void sbnddaq::PennMilliSliceWriter::finalizeLatestMicroSlice_()
{
  if (
#ifdef PENN_DONT_REBLOCK_USLICES
      header_()->microslice_count > 0 && 
#endif
      latest_microslice_ptr_.get() != 0) {
    int size_change = latest_microslice_ptr_->finalize();
    header_()->millislice_size -= size_change;
  }
}

sbnddaq::PennMilliSliceWriter::Header* sbnddaq::PennMilliSliceWriter::header_()
{
  return reinterpret_cast<Header *>(buffer_);
}

uint8_t* sbnddaq::PennMilliSliceWriter::data_(int index)
{
  uint8_t* ms_ptr = buffer_ + sizeof(Header);
  for (int idx = 0; idx < index; ++idx) {
    PennMicroSlice tmp_ms(ms_ptr);
    ms_ptr += tmp_ms.size();
  }
  return ms_ptr;
}
