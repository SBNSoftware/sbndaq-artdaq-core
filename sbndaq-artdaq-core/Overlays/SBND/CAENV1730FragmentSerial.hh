#ifndef sbndaq_artdaq_core_Overlays_Common_CAENV1730FragmentSerial_hh
#define sbndaq_artdaq_core_Overlays_Common_CAENV1730FragmentSerial_hh

#include "artdaq-core/Data/detail/RawFragmentHeader.hh"
#include "artdaq-core/Data/Fragment.hh"
#include "sbndaq-artdaq-core/Overlays/SBND/FragmentSerialBase.hh"
#include "sbndaq-artdaq-core/Overlays/Common/CAENV1730Fragment.hh"

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/vector.hpp>

namespace sbndaq {

  class CAENV1730FragmentSerial;
  std::ostream& operator<<(std::ostream&, CAENV1730FragmentSerial const&);

  class CAENV1730FragmentSerial : public sbndaq::FragmentSerialBase {
  public:
    CAENV1730FragmentMetadata metadata;
    CAENV1730Event event;
    std::vector<std::vector<uint16_t> > wvfmvec;
    
  private:
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive& ar, const unsigned int /*version*/)
    {
      ar & boost::serialization::base_object<FragmentSerialBase>(*this);
      ar & metadata;
      ar & event;
      ar & wvfmvec;
    }
  };
}

#endif
