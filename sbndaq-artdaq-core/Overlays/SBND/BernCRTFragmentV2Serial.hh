#ifndef sbndaq_artdaq_core_Overlays_Common_BernCRTFragmentV2Serial_hh
#define sbndaq_artdaq_core_Overlays_Common_BernCRTFragmentV2Serial_hh

#include "artdaq-core/Data/detail/RawFragmentHeader.hh"
#include "artdaq-core/Data/Fragment.hh"
#include "sbndaq-artdaq-core/Overlays/SBND/FragmentSerialBase.hh"
#include "sbndaq-artdaq-core/Overlays/Common/BernCRTFragmentV2.hh"

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/vector.hpp>

namespace sbndaq {

  class BernCRTFragmentV2Serial;
  std::ostream& operator<<(std::ostream&, BernCRTFragmentV2Serial const&);

  class BernCRTFragmentV2Serial : public sbndaq::FragmentSerialBase {
  public:
    size_t n_hits;
    size_t data_payload_size;
    BernCRTFragmentMetadataV2 metadata;
    std::vector<BernCRTHitV2> bern_crt_hits;
    
  private:
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive& ar, const unsigned int /*version*/)
    {
      ar & boost::serialization::base_object<FragmentSerialBase>(*this);
      ar & n_hits;
      ar & data_payload_size;
      ar & metadata;
      ar & bern_crt_hits;
    }
  };
}

#endif
