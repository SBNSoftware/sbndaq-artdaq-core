#ifndef sbnddaq_datatypes_Overlays_PennMilliSliceFragment_hh
#define sbnddaq_datatypes_Overlays_PennMilliSliceFragment_hh

#include "sbndaq-artdaq-core/Overlays/SBND/PennMilliSlice.hh"
#include "sbndaq-artdaq-core/Overlays/SBND/PennMicroSlice.hh"
#include "artdaq-core/Data/Fragment.hh"

namespace sbnddaq {
  class PennMilliSliceFragment;
}

class sbnddaq::PennMilliSliceFragment : public sbnddaq::PennMilliSlice {

public:

  // This constructor accepts an artdaq::Fragment that contains an existing
  // PennMilliSlice and allows the the data inside the PennMilliSlice to be accessed
  PennMilliSliceFragment(artdaq::Fragment const& frag);

protected:

  // returns a pointer to the header
  Header const* header_() const;

  // returns a pointer to the requested MicroSlice
  uint8_t* data_(int index) const;

private:

  artdaq::Fragment const& artdaq_fragment_;
};

#endif /* sbnddaq_datatypes_Overlays_PennMilliSliceFragment_hh */
