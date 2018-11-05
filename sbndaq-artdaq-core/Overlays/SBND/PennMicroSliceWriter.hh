#ifndef sbnddaq_datatypes_Overlays_PennMicroSliceWriter_hh
#define sbnddaq_datatypes_Overlays_PennMicroSliceWriter_hh

#include "sbndaq-artdaq-core/Overlays/SBND/PennMicroSlice.hh"

namespace sbnddaq {
  class PennMicroSliceWriter;
}

class sbnddaq::PennMicroSliceWriter : public sbnddaq::PennMicroSlice {

public:

  // This constructor creates an empty PennMicroSlice which can be filled
  // with the appropriate data
  PennMicroSliceWriter(uint8_t* address, uint32_t max_size_bytes);//, uint8_t sequence_id);

  //  void setSequenceId(uint8_t);

  // Adds the specified data sample PennMicroSlice.  Returns true
  // if it was successfully added, false if not (for example, if the
  // additional sample would overflow the maximum size of the
  // PennMicroSlice).
  //  bool addSample(uint16_t value);
  bool addData(uint16_t* data, uint16_t data_size);

  // Finalizes the PennMicroSlice.  This takes care of updating the
  // PennMicroSlice so that all internal pointers and data are consistent
  // with the data that has been added.  No more samples can be added
  // once the PennMicroSlice has been finalized.
  // This method returns the number of bytes that were reclaimed
  // when the maximum size for this PennMicroSlice was reduced to match
  // its actual current size.
  int32_t finalize();

protected:

  // returns a pointer to the header
  Header * header_();

  // returns a pointer to the first sample value
  uint16_t * data_();

  uint32_t max_size_bytes_;
};

#endif /* sbnddaq_datatypes_Overlays_PennMicroSliceWriter_hh */
